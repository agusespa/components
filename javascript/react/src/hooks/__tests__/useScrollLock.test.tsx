import { render } from '@testing-library/react';
import useScrollLock from '../useScrollLock';

const TestComponent = () => {
    useScrollLock();
    return <div>Test Component</div>;
};

describe('useScrollLock', () => {
    let addEventListenerSpy: jest.SpyInstance;
    let removeEventListenerSpy: jest.SpyInstance;

    beforeEach(() => {
        addEventListenerSpy = jest.spyOn(window, 'addEventListener');
        removeEventListenerSpy = jest.spyOn(window, 'removeEventListener');
    });

    afterEach(() => {
        addEventListenerSpy.mockRestore();
        removeEventListenerSpy.mockRestore();
    });

    it('should add scroll lock event listeners when mounted', () => {
        render(<TestComponent />);

        expect(addEventListenerSpy).toHaveBeenCalledWith('wheel', expect.any(Function), {
            passive: false,
        });
        expect(addEventListenerSpy).toHaveBeenCalledWith('touchmove', expect.any(Function), {
            passive: false,
        });
    });

    it('should remove scroll lock event listeners when unmounted', () => {
        const { unmount } = render(<TestComponent />);

        unmount();

        expect(removeEventListenerSpy).toHaveBeenCalledWith('wheel', expect.any(Function));
        expect(removeEventListenerSpy).toHaveBeenCalledWith('touchmove', expect.any(Function));
    });
});
