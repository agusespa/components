import { Dispatch, ReactElement, SetStateAction, useEffect, useRef, useState } from 'react';
import { CSSPosition, CursorPosition } from '../models/utils';
import { getUpdatedCSSPosition } from '../helpers/helpers';
import * as styles from './ContextMenu.module.css';

interface Props {
    position: CursorPosition;
    isShown: boolean;
    setIsShown: Dispatch<SetStateAction<boolean>>;
}

const ContextMenu = (props: Props): ReactElement => {
    const ref = useRef<HTMLDivElement>(null);
    const [updatedPosition, setUpdatedPosition] = useState<CSSPosition>({
        top: '0px',
        left: '0px',
        visibility: 'hidden',
    });

    useEffect(() => {
        let isPositionSet = false;

        const trySetPosition = (): void => {
            if (ref.current) {
                const { offsetWidth, offsetHeight } = ref.current;
                const position = getUpdatedCSSPosition(offsetWidth, offsetHeight, props.position);
                setUpdatedPosition(position);
                isPositionSet = true;
            }
        };

        trySetPosition();

        if (isPositionSet) return;

        trySetPosition();
    }, [props.position]);

    // useScrollLock();

    return (
        <>
            <div className={styles.backdrop} onClick={() => props.setIsShown(false)} />

            <div
                ref={ref}
                onClick={() => props.setIsShown(false)}
                className={styles.contextMenuContainer}
                style={updatedPosition}></div>
        </>
    );
};

export default ContextMenu;
