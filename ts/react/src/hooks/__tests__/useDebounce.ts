import { act, renderHook } from '@testing-library/react';
import useDebounce from '../useDebounce';

describe('useDebounce hook', () => {
    beforeEach(() => {
        jest.useFakeTimers();
    });

    afterEach(() => {
        jest.runOnlyPendingTimers();
        jest.useRealTimers();
    });

    it('should return the initial value immediately', () => {
        const { result } = renderHook(() => useDebounce('initial'));
        expect(result.current).toBe('initial');
    });

    it('should return the debounced value after the delay', () => {
        const { result, rerender } = renderHook(({ value }) => useDebounce(value), {
            initialProps: { value: 'initial' }
        });
        expect(result.current).toBe('initial');

        rerender({ value: 'updated' });

        expect(result.current).toBe('initial');

        act(() => {
            jest.advanceTimersByTime(500);
        });

        expect(result.current).toBe('updated');
    });

    it('should not update the debounced value before the delay', () => {
        const { result, rerender } = renderHook(({ value }) => useDebounce(value, 500), {
            initialProps: { value: 'initial' }
        });
        rerender({ value: 'updated' });

        act(() => {
            jest.advanceTimersByTime(300);
        });

        expect(result.current).toBe('initial');

        act(() => {
            jest.advanceTimersByTime(200);
        });

        expect(result.current).toBe('updated');
    });
});

