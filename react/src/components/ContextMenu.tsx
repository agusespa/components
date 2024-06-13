import {
    Dispatch,
    ReactElement,
    SetStateAction,
    MouseEvent,
    useEffect,
    useRef,
    useState,
} from 'react';
import { CSSPosition, CursorPosition } from '../models/utils';
import { getUpdatedCSSPosition } from '../helpers/helpers';
import * as styles from './ContextMenu.module.css';
import useScrollLock from '../hooks/useScrollLock';

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

    useScrollLock();

    useEffect(() => {
        if (ref?.current) {
            const { offsetWidth, offsetHeight } = ref.current;
            const position = getUpdatedCSSPosition(offsetWidth, offsetHeight, props.position);
            setUpdatedPosition(position);
        }
    }, [props.position]);

    function handleClickOutside(e: MouseEvent): void {
        e.preventDefault();
        props.setIsShown(false);
    }

    function handleClick(e: MouseEvent): void {
        e.preventDefault();
        props.setIsShown(false);
    }

    return (
        <>
            <div
                className={styles.backdrop}
                onClick={handleClickOutside}
                onContextMenu={handleClickOutside}
            />
            <div
                ref={ref}
                role="menu"
                onContextMenu={handleClick}
                onClick={handleClick}
                className={styles.contextMenuContainer}
                style={updatedPosition}></div>
        </>
    );
};

export default ContextMenu;
