import { describe, it, expect } from '@jest/globals';
import AsyncQueue from '../asyncQueue';

describe('AsyncQueue', () => {
    function task(result, delay) {
        return () =>
            new Promise(resolve => {
                setTimeout(() => {
                    resolve(result);
                }, delay);
            });
    }

    it('should process one single task', async () => {
        const queue = new AsyncQueue(2);

        const result = await queue.enqueue(task('Result', 30));

        expect(result).toEqual('Result');
    });

    it('should return the correct result for each task', async () => {
        const queue = new AsyncQueue(2);

        const tasks = [
            queue.enqueue(task('Result 1', 100)),
            queue.enqueue(task('Result 2', 200)),
            queue.enqueue(task('Result 3', 50)),
            queue.enqueue(task('Result 4', 150)),
        ];

        const results = await Promise.all(tasks);

        expect(results).toEqual(['Result 1', 'Result 2', 'Result 3', 'Result 4']);
    });

    it('should run tasks concurrently up to the specified concurrency limit', async () => {
        const queue = new AsyncQueue(2);
        let activeTasks = 0;
        let maxActiveTasks = 0;

        const trackConcurrency = async t => {
            activeTasks++;
            maxActiveTasks = Math.max(maxActiveTasks, activeTasks);

            const result = await t();

            activeTasks--;
            return result;
        };

        const tasks = [
            queue.enqueue(() => trackConcurrency(task('Result 1', 100))),
            queue.enqueue(() => trackConcurrency(task('Result 2', 200))),
            queue.enqueue(() => trackConcurrency(task('Result 3', 50))),
            queue.enqueue(() => trackConcurrency(task('Result 4', 150))),
        ];

        const results = await Promise.all(tasks);

        expect(maxActiveTasks).toBe(2);
        expect(results).toEqual(['Result 1', 'Result 2', 'Result 3', 'Result 4']);
    });

    it('should handle task errors properly', async () => {
        const queue = new AsyncQueue(2);

        function taskThatFails(delay) {
            return () =>
                new Promise((_, reject) => {
                    setTimeout(() => reject(new Error('Task failed')), delay);
                });
        }

        const tasks = [
            queue.enqueue(taskThatFails(100)).catch(err => err.message),
            queue.enqueue(taskThatFails(200)).catch(err => err.message),
        ];

        const results = await Promise.all(tasks);

        expect(results).toEqual(['Task failed', 'Task failed']);
    });

    it('should continue processing remaining tasks after a failure', async () => {
        const queue = new AsyncQueue(2);

        function taskThatFails(delay) {
            return () =>
                new Promise((_, reject) => {
                    setTimeout(() => reject(new Error('Task failed')), delay);
                });
        }

        const tasks = [
            queue.enqueue(task('Result 1', 100)),
            queue.enqueue(taskThatFails(200)).catch(err => err.message),
            queue.enqueue(task('Result 2', 50)),
            queue.enqueue(task('Result 3', 150)),
        ];

        const results = await Promise.all(tasks);

        expect(results).toEqual(['Result 1', 'Task failed', 'Result 2', 'Result 3']);
    });
});
