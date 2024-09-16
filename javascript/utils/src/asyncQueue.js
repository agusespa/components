/**
 *  Class representing an asynchronous task queue with a concurrency limit.
 */
class AsyncQueue {
    /**
     * Create an AsyncQueue.
     * @param {number} limit - The maximum number of tasks to run concurrently.
     */
    constructor(limit) {
        /**
         * @type {number}
         * @description The maximum number of concurrent tasks.
         */
        this.limit = limit;

        /**
         * @type {Array<Object>}
         * @description The task queue, storing tasks and their corresponding resolve/reject handlers.
         */
        this.queue = [];

        /**
         * @type {number}
         * @description The number of currently active (running) tasks.
         */
        this.activeCount = 0;
    }

    /**
     * Enqueue a new task to be executed by the queue.
     * @param {Function} task - A function that returns a promise representing an asynchronous task.
     * @returns {Promise<*>} A promise that resolves with the task's result when it completes.
     */
    enqueue(task) {
        return new Promise((resolve, reject) => {
            this.queue.push({ task, resolve, reject });
            this.runNext();
        });
    }

    /**
     * Run the next task in the queue if the concurrency limit allows.
     * @private
     */
    runNext() {
        if (this.activeCount < this.limit && this.queue.length > 0) {
            const { task, resolve, reject } = this.queue.shift();
            this.activeCount++;

            task()
                .then(result => {
                    this.activeCount--;
                    resolve(result);
                    this.runNext();
                })
                .catch(error => {
                    this.activeCount--;
                    reject(error);
                    this.runNext();
                });
        }
    }
}

export default AsyncQueue;
