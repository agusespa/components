import { useEffect } from 'react';

const useScrollLock = (): void => {
    useEffect(() => {
        const disableScroll = (): void => {
            document.body.style.overflow = 'hidden';
        };

        const enableScroll = (): void => {
            document.body.style.overflow = 'visible';
        };

        disableScroll();

        return () => {
            enableScroll();
        };
    }, []);
};

export default useScrollLock;
