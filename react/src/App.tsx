import { ReactElement, useState, MouseEvent } from 'react';
import * as styles from './App.module.css';
import { CursorPosition } from './models/utils';
import ContextMenu from './components/ContextMenu';

const App = (): ReactElement => {
    const [isContextMenuShown, setIsContextMenuShown] = useState(false);
    const [cursorPosition, setCursorPosition] = useState({} as CursorPosition);

    const handleSecondaryClick = (event: MouseEvent): void => {
        event.preventDefault();
        setIsContextMenuShown(true);
        setCursorPosition({ y: event.clientY, x: event.clientX });
    };

    return (
        <>
            <main className={styles.mainBody} onContextMenu={handleSecondaryClick}>
                <h1>React component samples</h1>
                {isContextMenuShown ? (
                    <ContextMenu
                        position={cursorPosition}
                        isShown={isContextMenuShown}
                        setIsShown={setIsContextMenuShown}
                    />
                ) : null}
            </main>
            <footer className={styles.footerContainer}>
                <p>This is for testing scroll locking</p>
            </footer>
        </>
    );
};

export default App;
