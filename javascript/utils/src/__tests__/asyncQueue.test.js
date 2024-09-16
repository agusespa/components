import { describe, it, expect } from '@jest/globals';
import AsyncQueue from '../asyncQueue';

describe('AsyncQueue', () => {
    it('should run tasks concurrently up to the specified concurrency limit', async () => {
        const queue = new AsyncQueue(2);
        let activeTasks = 0;
        let maxActiveTasks = 0;

        const trackConcurrency = async task => {
            activeTasks++;
            maxActiveTasks = Math.max(maxActiveTasks, activeTasks);

            const result = await task();

            activeTasks--;
            return result;
        };

        function task(delay, result) {
            return () =>
                new Promise(resolve => {
                    setTimeout(() => {
                        resolve(result);
                    }, delay);
                });
        }

        const tasks = [
            queue.enqueue(() => trackConcurrency(task(100, 'Result 1'))),
            queue.enqueue(() => trackConcurrency(task(200, 'Result 2'))),
            queue.enqueue(() => trackConcurrency(task(50, 'Result 3'))),
            queue.enqueue(() => trackConcurrency(task(150, 'Result 4'))),
        ];

        const results = await Promise.all(tasks);

        expect(maxActiveTasks).toBe(2);
        expect(results).toEqual(['Result 1', 'Result 2', 'Result 3', 'Result 4']);
    });

    it('should return the correct result for each task', async () => {
        const queue = new AsyncQueue(2);

        function task(result, delay) {
            return () =>
                new Promise(resolve => {
                    setTimeout(() => resolve(result), delay);
                });
        }

        const tasks = [
            queue.enqueue(task('Result 1', 100)),
            queue.enqueue(task('Result 2', 200)),
            queue.enqueue(task('Result 3', 50)),
            queue.enqueue(task('Result 4', 150)),
        ];

        const results = await Promise.all(tasks);

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

        function task(result, delay) {
            return () =>
                new Promise(resolve => {
                    setTimeout(() => resolve(result), delay);
                });
        }

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
