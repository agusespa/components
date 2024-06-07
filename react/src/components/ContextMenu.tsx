import { Dispatch, ReactElement, SetStateAction, useEffect, useRef, useState } from 'react';
import { CursorPosition } from '../models/utils';
import { getUpdatedCSSPosition } from '../helpers/helpers';
import * as styles from './ContextMenu.module.css';

interface Props {
    position: CursorPosition;
    isShown: boolean;
    setIsShown: Dispatch<SetStateAction<boolean>>;
}

const ContextMenu = (props: Props): ReactElement => {
    const [width, setWidth] = useState(0);
    const [height, setHeight] = useState(0);

    const ref = useRef<HTMLDivElement>(null);

    // useScrollLock();

    useEffect(() => {
        if (ref.current !== null) {
            setWidth(ref.current.offsetWidth);
            setHeight(ref.current.offsetHeight);
        }
    }, []);

    return (
        <>
            <div className={styles.backdrop} onClick={() => props.setIsShown(false)} />

            <div
                ref={ref}
                onClick={() => props.setIsShown(false)}
                className={styles.contextMenuContainer}
                style={getUpdatedCSSPosition(width, height, props.position)}></div>
        </>
    );
};

export default ContextMenu;
