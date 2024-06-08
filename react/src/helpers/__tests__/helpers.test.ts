import 'jest';
import { getUpdatedCSSPosition } from '../helpers';

describe('getUpdatedCSSPosition', () => {
    beforeAll(() => {
        window.innerHeight = 800; // Set the window height for testing
        window.innerWidth = 1200; // Set the window width for testing
    });

    it('should return correct CSS position when cursor position is within viewable screen', () => {
        const width = 100;
        const height = 200;
        const position = { x: 50, y: 50 };
        const expectedPosition = {
            top: `${position.y}px`,
            left: `${position.x}px`,
            visibility: 'visible',
        };

        expect(getUpdatedCSSPosition(width, height, position)).toEqual(expectedPosition);
    });

    it('should adjust top position when cursor position is below available viewable screen', () => {
        const width = 100;
        const height = 200;
        const position = { x: 50, y: window.innerHeight - 100 };
        const expectedPosition = {
            top: `${position.y - height}px`,
            left: `${position.x}px`,
            visibility: 'visible',
        };

        expect(getUpdatedCSSPosition(width, height, position)).toEqual(expectedPosition);
    });

    it('should adjust left position when cursor position is outside right edge of viewable screen', () => {
        const width = 100;
        const height = 200;
        const position = { x: window.innerWidth - 50, y: 50 };
        const expectedPosition = {
            top: `${position.y}px`,
            left: `${position.x - width}px`,
            visibility: 'visible',
        };

        expect(getUpdatedCSSPosition(width, height, position)).toEqual(expectedPosition);
    });
});
