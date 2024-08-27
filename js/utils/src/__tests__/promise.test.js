import { describe, it, expect } from '@jest/globals';
import { getAccessToken } from '../promise';

describe('getAccessToken', () => {

    it('should only call refreshToken once for multiple concurrent calls', async () => {
        const promises = Array(5)
            .fill()
            .map(() => getAccessToken());

        const results = await Promise.all(promises);
        expect(results[0]).toBe('Success!');
    });
});
