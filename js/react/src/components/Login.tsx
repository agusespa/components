import { ChangeEventHandler, FormEvent, ReactElement, useState } from 'react';
import * as styles from './Login.module.css';
import HttpClient from '../utils/HttpClient';

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
interface UserData {
    firstName: string;
    lastName: string;
}

const Login = ({ httpClient }: Props): ReactElement => {
    const [formData, setFormData] = useState<FormData>({ username: '', password: '' });
    const [id, setId] = useState('');

    const handleSubmit = async (e: FormEvent<HTMLFormElement>): Promise<void> => {
        e.preventDefault();
        try {
            const r = await getLogin(formData);
            setId(r.user_id);
        } catch (err) {
            console.error(err);
        }
    };

    const handleFormDataChange: ChangeEventHandler<HTMLInputElement> = (event): void => {
        const { name, value } = event.target;
        setFormData(prevFormData => {
            return {
                ...prevFormData,
                [name]: value,
            };
        });
    };

    const handleData = async () => {
        try {
            const r = await getUserData(id);
            console.log(r);
        } catch (err) {
            console.error(err);
        }
    };

    async function getLogin(formData: FormData): Promise<UserAuth> {
        const credentials = btoa(`${formData.username}:${formData.password}`);
        const options: RequestInit = {
            method: 'GET',
            headers: {
                Accept: 'application/json+cookie',
                Authorization: `Basic ${credentials}`,
            },
        };

        return httpClient.usePublic<UserAuth>('/authapi/login', options);
    }

    async function getUserData(id: string): Promise<UserData> {
        const options: RequestInit = {
            method: 'GET',
            credentials: 'include',
        };

        return httpClient.useSecure<UserData>(`/authapi/user?id=${id}`, options);
    }

    return (
        <div className={styles.container}>
            <h2>Login</h2>
            <form onSubmit={handleSubmit}>
                <div>
                    <label htmlFor="username">Username:</label>
                    <input
                        type="text"
                        name="username"
                        value={formData.username}
                        onChange={handleFormDataChange}
                    />
                </div>
                <div>
                    <label htmlFor="password">Password:</label>
                    <input
                        type="password"
                        name="password"
                        value={formData.password}
                        onChange={handleFormDataChange}
                    />
                </div>
                <button type="submit">Login</button>
            </form>
            <button onClick={handleData}>Fetch User Data</button>
        </div>
    );
};

export default Login;
