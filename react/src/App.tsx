import { ReactElement } from 'react';
import * as styles from './App.module.css';

const App = (): ReactElement => {
    return (
        <div>
            <main className={styles.mainBody}>
                <h1>React component samples</h1>
            </main>
            <footer className={styles.footerContainer}>
                <p>This is for testing scroll</p>
            </footer>
        </div>
    );
};

export default App;
