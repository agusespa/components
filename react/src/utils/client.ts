export function deleteAllCookies(): void {
    const cookieArr = document.cookie.split(';');

    cookieArr.forEach(cookie => {
        const parts = cookie.split('=');
        const key = decodeURIComponent(parts[0].trim());

        document.cookie = `${key}=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;`;
    });
}

export function getCookieValue(name: string): string | undefined {
    const cookieArr = document.cookie.split('; ');

    const cookie = cookieArr.find(c => c.startsWith(`${name}=`));

    if (cookie === undefined) return;

    return cookie.split('=')[1];
}
