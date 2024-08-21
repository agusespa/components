import { render, screen } from '@testing-library/react';
import ContextMenu from '../ContextMenu';

const mockSetIsShown = jest.fn();

describe('ContextMenu component', () => {
    it('renders the div with the ref and has visibility visible', () => {
        const position = { x: 100, y: 100 };
        const isShown = true;

        render(<ContextMenu position={position} isShown={isShown} setIsShown={mockSetIsShown} />);

        const contextMenuContainer = screen.getByRole('menu');
        expect(contextMenuContainer).toBeInTheDocument();
        expect(contextMenuContainer).toHaveStyle('visibility: visible');
        expect(contextMenuContainer.style.left).toBe('100px');
        expect(contextMenuContainer.style.top).toBe('100px');
    });
});
