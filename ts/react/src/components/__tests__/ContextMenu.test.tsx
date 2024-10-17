import React from 'react';
import { render, screen, fireEvent } from '@testing-library/react';
import ContextMenu from '../ContextMenu';

const mockSetIsShown = jest.fn();

const defaultProps = {
    position: { x: 100, y: 100 },
    isShown: true,
    setIsShown: mockSetIsShown,
};

describe('ContextMenu component', () => {
    beforeEach(() => {
        mockSetIsShown.mockClear();
    });

    it('renders the div with the ref and has visibility visible when isShown is true', () => {
        render(<ContextMenu {...defaultProps} />);
        const contextMenuContainer = screen.getByRole('menu');
        expect(contextMenuContainer).toBeInTheDocument();
        expect(contextMenuContainer).toHaveStyle('visibility: visible');
        expect(contextMenuContainer).toHaveStyle({ left: '100px' });
        expect(contextMenuContainer).toHaveStyle({ top: '100px' });
    });

    const TestWrapper = ({ children }: { children: React.ReactNode }) => {
        const [isShown, setIsShown] = React.useState(true);
        return isShown
            ? React.cloneElement(children as React.ReactElement, { isShown, setIsShown })
            : null;
    };

    it('removes the component from the DOM when clicking outside the menu', () => {
        render(
            <TestWrapper>
                <ContextMenu {...defaultProps} />
            </TestWrapper>
        );
        expect(screen.getByRole('menu')).toBeInTheDocument();

        const backdrop = screen.getByTestId('backdrop');
        fireEvent.click(backdrop);

        expect(screen.queryByTestId('backdrop')).not.toBeInTheDocument();
        expect(screen.queryByRole('menu')).not.toBeInTheDocument();
    });

    it('removes the component from the DOM when right-clicking outside the menu', () => {
        render(
            <TestWrapper>
                <ContextMenu {...defaultProps} />
            </TestWrapper>
        );
        expect(screen.getByRole('menu')).toBeInTheDocument();

        const backdrop = screen.getByTestId('backdrop');
        fireEvent.contextMenu(backdrop);

        expect(backdrop).not.toBeInTheDocument();
        expect(screen.queryByRole('menu')).not.toBeInTheDocument();
    });
});
