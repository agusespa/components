let tokenPromise = null;
let tokenExpiration = 0;
const MAX_RETRIES = 2;
const RETRY_DELAY = 500;

export async function getAccessToken() {
    const now = Date.now();
    if (!tokenPromise || now >= tokenExpiration) {
        tokenPromise = retryRefreshToken().catch(error => {
            tokenPromise = null;
            throw error;
        });
        tokenExpiration = now + 3600000;
    }
    const token = await tokenPromise;
    return token;
}

async function retryRefreshToken(retries = 0) {
    try {
        return await refreshToken();
    } catch (error) {
        if (retries < MAX_RETRIES) {
            console.log(`Refresh token attempt ${retries + 1} failed. Retrying...`);
            await new Promise(resolve => setTimeout(resolve, RETRY_DELAY));
            return retryRefreshToken(retries + 1);
        } else {
            throw error;
        }
    }
}

export function refreshToken() {
    console.log('Refreshing token...');
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            const success = true;
            if (success) {
                resolve('Success!');
            } else {
                reject(new Error('Failed to refresh token'));
            }
        }, 500);
    });
}
