import { deleteAllCookies, getCookieValue } from '../cookies';
import { describe, it, expect, beforeEach } from '@jest/globals';

describe('getCookieValue', () => {
    beforeEach(() => {
        document.cookie = 'username=JohnDoe';
        document.cookie = 'session_id=12345';
        document.cookie = 'preferences=darkMode';
    });

    it('should return the value of the specified cookie', () => {
        expect(getCookieValue('username')).toBe('JohnDoe');
    });

    it('should return undefined if the cookie is not found', () => {
        expect(getCookieValue('nonexistent')).toBeUndefined();
    });

    it('should delete all cookies', () => {
        deleteAllCookies();
        expect(document.cookie).not.toContain('username=JohnDoe');
        expect(document.cookie).not.toContain('session_id=12345');
        expect(document.cookie).not.toContain('preferences=darkMode');
        expect(document.cookie).toBe('');
    });
});
