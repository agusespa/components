import HttpError from './HttpError';

class HttpClient {
    private baseUrl: string;
    private maxRetries: number;

    constructor(url: string, maxRetries = 0) {
        this.baseUrl = url;
        this.maxRetries = maxRetries;
    }

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

    public async useSecure<T>(endpoint: string, options: RequestInit): Promise<T> {
        const fetchRequest = async (): Promise<T> => {
            try {
                const res = await fetch(this.baseUrl + endpoint, options);

                if (res.status === 401) {
                    if (await this.refreshAuth()) return await this.useSecure(endpoint, options);
                    else throw Error('failed refreashig auth');
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

    private async refreshAuth(): Promise<boolean> {
        const fetchRequest = async (): Promise<boolean> => {
            try {
                const res = await fetch(this.baseUrl + '/authapi/refresh', {
                    method: 'GET',
                    credentials: 'include',
                });

                if (!res.ok) {
                    throw new HttpError(res.status, res.statusText);
                }

                const setCookieHeader = res.headers.get('set-cookie');
                if (setCookieHeader?.includes('access_token')) return true;
                else throw new HttpError(401, 'Unauthorized');
            } catch (err) {
                if (err instanceof HttpError) throw err;
                else throw new HttpError(-1, 'unknown fetch error', String(err));
            }
        };

        return this.retry<boolean>(fetchRequest);
    }

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

    private getExponentialBackoffTime(tries: number): number {
        const baseDelay = 1000;
        const jitter = Math.random() * 500;
        return baseDelay * Math.pow(2, tries - 1) + jitter;
    }

    private async sleep(ms: number): Promise<void> {
        return new Promise(resolve => setTimeout(resolve, ms));
    }
}

export default HttpClient;
