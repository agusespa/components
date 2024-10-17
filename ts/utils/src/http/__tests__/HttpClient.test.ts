import { describe, it, expect, beforeEach } from '@jest/globals';

import HttpClient from '../HttpClient.ts';
import HttpError from '../HttpError.ts';

const mockFetch = jest.fn();
global.fetch = mockFetch;

describe('HttpClient', () => {
    let httpClient: HttpClient;

    beforeEach(() => {
        httpClient = new HttpClient('https://api.example.com', 2);
        jest.clearAllMocks();
    });

    describe('usePublic', () => {
        it('should make a successful request', async () => {
            const mockResponse = { data: 'test' };
            mockFetch.mockResolvedValueOnce({
                ok: true,
                json: jest.fn().mockResolvedValueOnce(mockResponse),
            });

            const result = await httpClient.usePublic<{ data: string }>('/public', {
                method: 'GET',
            });
            expect(result).toEqual(mockResponse);
            expect(mockFetch).toHaveBeenCalledWith('https://api.example.com/public', {
                method: 'GET',
            });
        });

        it('should throw HttpError for non-ok responses', async () => {
            mockFetch.mockResolvedValueOnce({
                ok: false,
                status: 404,
                statusText: 'Not Found',
            });

            await expect(httpClient.usePublic('/public', { method: 'GET' })).rejects.toThrow(
                HttpError
            );
            expect(mockFetch).toHaveBeenCalledTimes(1);
        });

        it('should retry on retryable errors', async () => {
            mockFetch
                .mockRejectedValueOnce(new HttpError(500, 'Internal Server Error'))
                .mockResolvedValueOnce({
                    ok: true,
                    json: jest.fn().mockResolvedValueOnce({ data: 'retry success' }),
                });

            const promise = httpClient.usePublic<{ data: string }>('/public', {
                method: 'GET',
            });

            const result = await promise;
            expect(result).toEqual({ data: 'retry success' });
            expect(mockFetch).toHaveBeenCalledTimes(2);
        });
    });

    describe('useSecure', () => {
        it('should make a successful request', async () => {
            const mockResponse = { data: 'secure' };
            mockFetch.mockResolvedValueOnce({
                ok: true,
                json: jest.fn().mockResolvedValueOnce(mockResponse),
            });

            const result = await httpClient.useProtected<{ data: string }>('/secure', {
                method: 'GET',
            });
            expect(result).toEqual(mockResponse);
            expect(mockFetch).toHaveBeenCalledWith('https://api.example.com/secure', {
                method: 'GET',
            });
        });

        it('should refresh auth and retry on 401', async () => {
            mockFetch
                .mockResolvedValueOnce({
                    ok: false,
                    status: 401,
                    statusText: 'Unauthorized',
                })
                .mockResolvedValueOnce({
                    ok: true,
                    headers: new Headers({ 'set-cookie': 'access_token=new_token' }),
                })
                .mockResolvedValueOnce({
                    ok: true,
                    json: jest.fn().mockResolvedValueOnce({ data: 'auth refreshed' }),
                });

            const result = await httpClient.useProtected<{ data: string }>('/secure', {
                method: 'GET',
            });

            expect(result).toEqual({ data: 'auth refreshed' });
            expect(mockFetch).toHaveBeenCalledTimes(3);
        });
    });

    describe('refreshAuth', () => {
        it('should successfully refresh auth', async () => {
            mockFetch.mockResolvedValueOnce({
                ok: true,
                headers: new Headers({ 'set-cookie': 'access_token=new_token' }),
            });

            const result = await (httpClient as any).refreshAuth();
            expect(result).toBe(true);
            expect(mockFetch).toHaveBeenCalledWith('https://api.example.com/authapi/refresh', {
                method: 'GET',
                credentials: 'include',
                headers: { Accept: 'application/json+cookie' },
            });
        });

        it('should throw HttpError when refresh fails', async () => {
            mockFetch.mockResolvedValueOnce({
                ok: false,
                status: 401,
                statusText: 'Unauthorized',
            });

            await expect((httpClient as any).refreshAuth()).rejects.toThrow(HttpError);
        });
    });

    describe('retry', () => {
        it('should retry the specified number of times', async () => {
            const mockFn = jest
                .fn()
                .mockRejectedValueOnce(new HttpError(500, 'Internal Server Error'))
                .mockRejectedValueOnce(new HttpError(500, 'Internal Server Error'))
                .mockResolvedValueOnce({ data: 'success' });

            const result = await (httpClient as any).retry(mockFn);
            expect(result).toEqual({ data: 'success' });
            expect(mockFn).toHaveBeenCalledTimes(3);
        });

        it('should throw after max retries', async () => {
            const mockFn = jest.fn().mockRejectedValue(new HttpError(500, 'Internal Server Error'));

            await expect((httpClient as any).retry(mockFn)).rejects.toThrow(HttpError);
            expect(mockFn).toHaveBeenCalledTimes(3);
        });
    });
});
