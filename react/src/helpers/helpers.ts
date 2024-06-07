import { CSSPosition, CursorPosition } from '../models/utils';

export const getUpdatedCSSPosition = (
    width: number,
    height: number,
    position: CursorPosition
): CSSPosition => {
    const viewableScreenMaxHeight = window.innerHeight - height - 10;
    const viewableScreenMaxWidth = window.innerWidth - width - 10;

    const styles: CSSPosition = {
        top: `${position.y}px`,
        left: `${position.x}px`,
    };

    if (position.y > viewableScreenMaxHeight) styles.top = `${position.y - height}px`;

    if (position.x > viewableScreenMaxWidth) styles.left = `${position.x - width}px`;

    return styles;
};
