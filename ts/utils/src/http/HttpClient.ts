/**
 * @file HttpClient.ts
 * @description Provides an HttpClient class for making HTTP requests with retry and authentication capabilities.
 */

import HttpError from './HttpError.ts';

/**
 * HttpClient class for making public and protected HTTP requests with retry attempts.
 */
class HttpClient {
    private baseUrl: string;
    private maxRetries: number;

    /**
     * Creates an instance of HttpClient.
     * @param url - The base URL for all requests.
     * @param maxRetries - Maximum number of retry attempts for failed requests. Default is 0.
     */
    constructor(url: string, maxRetries = 0) {
        this.baseUrl = url;
        this.maxRetries = maxRetries;
    }

    /**
     * Makes a public HTTP request.
     * @template T - The expected return type of the request.
     * @param endpoint - The API endpoint to request.
     * @param options - Request options to be passed to fetch.
     * @returns A promise that resolves with the response data.
     * @throws {HttpError} If the request fails.
     */
    public async usePublic<T>(endpoint: string, options: RequestInit): Promise<T> {
        const fetchRequest = async (): Promise<T> => {
            try {
                const res = await fetch(this.baseUrl + endpoint, options);
                if (!res.ok) {
                    throw new HttpError(res.status, res.statusText);
                }
                return await res.json();
            } catch (err) {
                if (err instanceof HttpError) throw err;
                else throw new HttpError(-1, 'unknown fetch error', String(err));
            }
        };
        return this.retry<T>(fetchRequest);
    }

    /**
     * Makes a protected HTTP request with authentication refresh capability.
     * @template T - The expected return type of the request.
     * @param endpoint - The API endpoint to request.
     * @param options - Request options to be passed to fetch.
     * @returns A promise that resolves with the response data.
     * @throws {HttpError} If the request fails or authentication refresh fails.
     */
    public async useProtected<T>(endpoint: string, options: RequestInit): Promise<T> {
        const fetchRequest = async (): Promise<T> => {
            try {
                const res = await fetch(this.baseUrl + endpoint, options);
                if (res.status === 401) {
                    try {
                        const refresh = await this.refreshAuth();
                        if (refresh) return await this.useProtected(endpoint, options);
                    } catch (err) {
                        if (err instanceof HttpError) throw err;
                        else throw new HttpError(-1, 'unknown fetch error', String(err));
                    }
                }
                if (!res.ok) {
                    throw new HttpError(res.status, res.statusText);
                }
                return await res.json();
            } catch (err) {
                if (err instanceof HttpError) throw err;
                else throw new HttpError(-1, 'unknown fetch error', String(err));
            }
        };
        return this.retry<T>(fetchRequest);
    }

    /**
     * Attempts to refresh the authentication token.
     * @returns A promise that resolves to true if refresh is successful.
     * @throws {HttpError} If the refresh request fails.
     */
    private async refreshAuth(): Promise<boolean> {
        const fetchRequest = async (): Promise<boolean> => {
            try {
                const res = await fetch(this.baseUrl + '/authapi/refresh', {
                    method: 'GET',
                    headers: {
                        Accept: 'application/json+cookie',
                    },
                    credentials: 'include',
                });
                if (!res.ok) {
                    throw new HttpError(res.status, res.statusText);
                }
                return true;
            } catch (err) {
                if (err instanceof HttpError) throw err;
                else throw new HttpError(-1, 'unknown fetch error', String(err));
            }
        };
        return this.retry<boolean>(fetchRequest);
    }

    /**
     * Retries a function with exponential backoff.
     * @template T - The expected return type of the function.
     * @param fn - The function to retry.
     * @returns A promise that resolves with the function's return value.
     * @throws {HttpError} If all retry attempts fail.
     */
    private async retry<T>(fn: () => Promise<T>): Promise<T> {
        let tries = 0;
        let res: T | undefined;
        while (tries < this.maxRetries + 1) {
            tries++;
            try {
                res = await fn();
                break;
            } catch (err) {
                if (err instanceof HttpError && !err.isRetryable()) {
                    throw err;
                }
                if (tries > this.maxRetries) {
                    if (err instanceof HttpError) throw err;
                    else throw new HttpError(-1, 'unknown fetch error', String(err));
                }
                await this.sleep(this.getExponentialBackoffTime(tries));
            }
        }
        if (res === undefined) throw new HttpError(-1, 'unknown fetch error');
        return res;
    }

    /**
     * Calculates the exponential backoff time for retries.
     * @param tries - The current number of retry attempts.
     * @returns The calculated backoff time in milliseconds.
     */
    private getExponentialBackoffTime(tries: number): number {
        const baseDelay = 1000;
        const jitter = Math.random() * 500;
        return baseDelay * Math.pow(2, tries - 1) + jitter;
    }

    /**
     * Pauses execution for a specified duration.
     * @param ms - The duration to sleep in milliseconds.
     * @returns A promise that resolves after the specified duration.
     */
    private async sleep(ms: number): Promise<void> {
        return new Promise(resolve => setTimeout(resolve, ms));
    }
}

export default HttpClient;
