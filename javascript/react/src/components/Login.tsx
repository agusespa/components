import { ChangeEventHandler, FormEvent, ReactElement, useState } from 'react';
import * as styles from './Login.module.css';
import HttpClient from '../utils/HttpClient';
import HttpError from '../utils/HttpError';

interface Props {
    httpClient: HttpClient;
}

interface FormData {
    username: string;
    password: string;
}
interface UserAuth {
    user_id: string;
}

const Login = ({ httpClient }: Props): ReactElement => {
    const [formData, setFormData] = useState<FormData>({ username: '', password: '' });
    const [errorMessage, setErrorMessage] = useState('');

    const handleFormDataChange: ChangeEventHandler<HTMLInputElement> = (event): void => {
        const { id, value } = event.target;
        setFormData(prevFormData => {
            return {
                ...prevFormData,
                [id]: value,
            };
        });
    };

    const handleSubmitClick = (e: FormEvent<HTMLFormElement>): void => {
        e.preventDefault();
        setErrorMessage('');
        void submit(formData);
    };

    async function submit(data: FormData): Promise<void> {
        try {
            await getLogin(data);
        } catch (err) {
            const errorCause = err instanceof HttpError ? err.statusText : 'unknown';
            setErrorMessage('Login failed: ' + errorCause);
        }
    }

    async function getLogin(data: FormData): Promise<UserAuth> {
        const formData = new window.FormData();
        formData.append('username', data.username);
        formData.append('password', data.password);

        const options: RequestInit = {
            method: 'POST',
            headers: {
                Accept: 'application/json+cookie',
            },
            body: formData,
        };

        return httpClient.usePublic<UserAuth>('/authapi/login', options);
    }

    return (
        <div className={styles.container}>
            <h2>Login</h2>
            <form onSubmit={handleSubmitClick}>
                <div>
                    <label htmlFor="username">Username:</label>
                    <input
                        type="text"
                        id="username"
                        value={formData.username}
                        onChange={handleFormDataChange}
                    />
                </div>
                <div>
                    <label htmlFor="password">Password:</label>
                    <input
                        type="password"
                        id="password"
                        value={formData.password}
                        onChange={handleFormDataChange}
                    />
                </div>
                <button type="submit">Login</button>
            </form>
            {errorMessage !== '' ? <p>{errorMessage}</p> : null}
        </div>
    );
};

export default Login;
