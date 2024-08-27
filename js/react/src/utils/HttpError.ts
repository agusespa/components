class HttpError extends Error {
    public statusCode: number;
    public statusText: string;

    constructor(statusCode: number, statusText: string, message?: string) {
        super(message ?? `HTTP Error ${statusCode}: ${statusText}`);
        this.statusCode = statusCode;
        this.statusText = statusText;

        Object.setPrototypeOf(this, HttpError.prototype);
    }

    public isRetryable(): boolean {
        if (this.statusCode >= 500 && this.statusCode < 600) {
            return true;
        }

        if (this.statusCode === 429) {
            return true;
        }

        return false;
    }
}

export default HttpError;
