import { CSSPosition, CursorPosition } from "./types";

export const getUpdatedCSSPosition = (
    width: number,
    height: number,
    position: CursorPosition
): CSSPosition => {
    const styles: CSSPosition = {
        top: `${position.y}px`,
        left: `${position.x}px`,
        visibility: 'visible'
    };

    const viewableScreenMaxHeight = window.innerHeight - height - 10;
    const viewableScreenMaxWidth = window.innerWidth - width - 10;

    if (position.y > viewableScreenMaxHeight) styles.top = `${position.y - height}px`;

    if (position.x > viewableScreenMaxWidth) styles.left = `${position.x - width}px`;

    return styles;
};
