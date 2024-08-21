import { describe, it, expect } from '@jest/globals';
import { createTask, createQueue } from '../asyncQueue';

describe('createQueue', () => {
    it('should execute all tasks with a maximum of 4 workers', async () => {
        const tasks = [createTask(1), createTask(2), createTask(3), createTask(4), createTask(5)];
        const results = await createQueue(tasks);
        expect(results).toEqual([1, 2, 3, 4, 5]);
    });

    it('should handle rejections', async () => {
        const tasks = [createTask(1), createTask(2), createTask(6), createTask(4), createTask(5)];
        const results = await createQueue(tasks);
        expect(results).toEqual([1, 2, new Error('sorry'), 4, 5]);
    });

    it('should handle a reduced number of workers', async () => {
        const tasks = [createTask(1), createTask(2), createTask(3), createTask(4), createTask(5)];
        const results = await createQueue(tasks, 2);
        expect(results).toEqual([1, 2, 3, 4, 5]);
    });
});

describe('createTask', () => {
    it('should return a promise that resolves after the given value in milliseconds', async () => {
        const task = createTask(100);
        const result = await task();
        expect(result).toEqual(100);
    });

    it('should return a promise that rejects if the value is 6', async () => {
        const task = createTask(6);
        await expect(task()).rejects.toEqual(new Error('sorry'));
    });
});
