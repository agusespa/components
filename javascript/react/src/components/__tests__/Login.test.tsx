import { render, screen, fireEvent, waitFor } from '@testing-library/react';
import '@testing-library/jest-dom';
import HttpClient from '../../utils/HttpClient';
import Login from '../Login';

jest.mock('../../utils/HttpClient');

describe('Login Component', () => {
    let mockHttpClient: jest.Mocked<HttpClient>;

    beforeEach(() => {
        mockHttpClient = {
            usePublic: jest.fn(),
        } as unknown as jest.Mocked<HttpClient>;
    });

    it('renders login form', () => {
        render(<Login httpClient={mockHttpClient} />);

        expect(screen.getByLabelText('Username:')).toBeInTheDocument();
        expect(screen.getByLabelText('Password:')).toBeInTheDocument();
        expect(screen.getByRole('button', { name: 'Login' })).toBeInTheDocument();
    });

    it('updates form data on input change', () => {
        render(<Login httpClient={mockHttpClient} />);

        const usernameInput = screen.getByLabelText('Username:') as HTMLInputElement;
        const passwordInput = screen.getByLabelText('Password:') as HTMLInputElement;

        fireEvent.change(usernameInput, { target: { value: 'testuser' } });
        fireEvent.change(passwordInput, { target: { value: 'password123' } });

        expect(usernameInput.value).toBe('testuser');
        expect(passwordInput.value).toBe('password123');
    });

    it('displays error message when login fails due to unknown error', async () => {
        mockHttpClient.usePublic.mockRejectedValue(new Error());

        render(<Login httpClient={mockHttpClient} />);

        const usernameInput = screen.getByLabelText('Username:');
        const passwordInput = screen.getByLabelText('Password:');
        const submitButton = screen.getByRole('button', { name: 'Login' });

        fireEvent.change(usernameInput, { target: { value: 'testuser' } });
        fireEvent.change(passwordInput, { target: { value: 'password' } });
        fireEvent.click(submitButton);

        await waitFor(() => {
            expect(mockHttpClient.usePublic).toHaveBeenCalled();
        });

        const errorMessage = screen.getByText('Login failed: unknown');
        expect(errorMessage).toBeInTheDocument();
    });

    it('calls getLogin with correct data on form submission', async () => {
        const mockUserAuth = { user_id: '123' };
        mockHttpClient.usePublic.mockResolvedValue(mockUserAuth);

        render(<Login httpClient={mockHttpClient} />);

        const usernameInput = screen.getByLabelText('Username:') as HTMLInputElement;
        const passwordInput = screen.getByLabelText('Password:') as HTMLInputElement;
        fireEvent.change(usernameInput, { target: { value: 'testuser' } });
        fireEvent.change(passwordInput, { target: { value: 'password123' } });
        expect(usernameInput.value).toBe('testuser');
        expect(passwordInput.value).toBe('password123');

        const submitButton = screen.getByRole('button', { name: 'Login' });
        fireEvent.click(submitButton);

        await waitFor(() => {
            expect(mockHttpClient.usePublic).toHaveBeenCalledWith(
                '/authapi/login',
                expect.objectContaining({
                    method: 'POST',
                    headers: {
                        Accept: 'application/json+cookie',
                    },
                })
            );
        });

        const [, options] = mockHttpClient.usePublic.mock.calls[0];
        const formData = options.body as FormData;
        expect(formData.get('username')).toBe('testuser');
        expect(formData.get('password')).toBe('password123');
    });
});
