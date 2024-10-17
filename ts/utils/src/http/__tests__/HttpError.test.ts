import { describe, it, expect } from '@jest/globals';
import HttpError from '../HttpError.ts';

describe('HttpError', () => {
    it('should create an instance with correct properties', () => {
        const error = new HttpError(404, 'Not Found');
        expect(error).toBeInstanceOf(HttpError);
        expect(error).toBeInstanceOf(Error);
        expect(error.statusCode).toBe(404);
        expect(error.statusText).toBe('Not Found');
        expect(error.message).toBe('HTTP Error 404: Not Found');
    });

    it('should use custom message if provided', () => {
        const error = new HttpError(500, 'Internal Server Error', 'Custom error message');
        expect(error.message).toBe('Custom error message');
    });

    describe('isRetryable', () => {
        it('should return true for 5xx errors', () => {
            const error500 = new HttpError(500, 'Internal Server Error');
            const error502 = new HttpError(502, 'Bad Gateway');
            const error599 = new HttpError(599, 'Network Connect Timeout Error');

            expect(error500.isRetryable()).toBe(true);
            expect(error502.isRetryable()).toBe(true);
            expect(error599.isRetryable()).toBe(true);
        });

        it('should return true for 429 Too Many Requests', () => {
            const error = new HttpError(429, 'Too Many Requests');
            expect(error.isRetryable()).toBe(true);
        });

        it('should return false for non-retryable errors', () => {
            const error400 = new HttpError(400, 'Bad Request');
            const error404 = new HttpError(404, 'Not Found');
            const error301 = new HttpError(301, 'Moved Permanently');

            expect(error400.isRetryable()).toBe(false);
            expect(error404.isRetryable()).toBe(false);
            expect(error301.isRetryable()).toBe(false);
        });
    });
});
