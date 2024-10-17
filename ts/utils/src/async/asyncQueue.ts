/**
 *  Class representing an asynchronous task queue with a concurrency limit.
 */
class AsyncQueue<T> {
    limit: number;
    activeCount: number = 0;
    private queue: {
        task: () => Promise<T>;
        resolve: (value: T) => void;
        reject: (reason?: any) => void;
    }[] = [];

    constructor(limit: number) {
        this.limit = limit;
    }

    /**
     * Enqueue a new task to be executed by the queue.
     */
    enqueue(task: () => Promise<T>): Promise<T> {
        return new Promise<T>((resolve, reject) => {
            this.queue.push({ task, resolve, reject });
            this.runNext();
        });
    }

    private runNext(): void {
        if (this.activeCount < this.limit && this.queue.length > 0) {
            const e = this.queue.shift();
            if (e) {
                this.activeCount++;
                e.task()
                    .then((result: T) => {
                        this.activeCount--;
                        e.resolve(result);
                        this.runNext();
                    })
                    .catch((error: Error) => {
                        this.activeCount--;
                        e.reject(error);
                        this.runNext();
                    });
            }
        }
    }
}

export default AsyncQueue;
