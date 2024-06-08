import { useEffect } from 'react';

const useScrollLock = (): void => {
    useEffect(() => {
        const preventDefault = (e: Event): void => e.preventDefault();

        window.addEventListener('wheel', preventDefault, { passive: false });
        window.addEventListener('touchmove', preventDefault, { passive: false });

        return (): void => {
            window.removeEventListener('wheel', preventDefault);
            window.removeEventListener('touchmove', preventDefault);
        };
    }, []);
};

export default useScrollLock;
