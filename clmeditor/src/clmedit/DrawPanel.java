package clmedit;

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

public class DrawPanel extends JComponent {

    private final int hmarg = 20 * 48;
    private final int vmarg = 22 * 24;
    private int pom = 0;

    Rectangle currentSelection = new Rectangle(0, 0, 1, 1);
    DrawPanelMouseMotion mouseMotion = new DrawPanelMouseMotion();

    private int currentElement = CalominConstants.ROCK_FULL;
    private int currentCave = 0;
    private final Image[] images = new Image[30];
    private int[][][] caveData;
    private boolean selectionMode = false;

    public void setCaveData(int[][][] data) {
        caveData = data;
    }

    public void setCurrentCave(int cave) {
        currentCave = cave;
        repaint();
    }

    public void setCurrentElement(int element) {

        boolean doRepaint = false;

        if (selectionMode == true) {
            doRepaint = true;
        }
        selectionMode = false;

        if (currentElement != element) {
            doRepaint = true;
        }

        currentElement = element;
        if (doRepaint == true) {
            repaint();
        }

    }

    public void placeCurrentElementAt(int x, int y) {
        caveData[currentCave][y][x] = currentElement;
        repaint();
    }

    public void setSelectionMode(boolean mode) {
        if (selectionMode != mode) {
            selectionMode = mode;
            repaint();
        }
    }

    public boolean getSelectionMode() {
        return selectionMode;
    }

    public void paintComponent(Graphics g) {

        g.setColor(Color.black);
        g.fillRect(0, 0, this.getWidth(), this.getHeight());

        for (int y = 0; y < 22; y++) {
            for (int x = 0; x < 20; x++) {
                int elem = caveData[currentCave][y][x];
                g.drawImage(images[elem], x * 48, y * 24, this);
            }
        }

        /*Draw grid*/
        g.setColor(Color.white);
        for (int i = 0; i < 21; i++) {
            pom = i * 48;
            g.drawLine(pom, 0, pom, vmarg);
        }
        for (int i = 0; i < 23; i++) {
            pom = i * 24;
            g.drawLine(0, pom, hmarg, pom);
        }

        /*Draw currently selected element*/
        g.setColor(Color.white);
        if (selectionMode == true) {
            g.drawString("Rectangular selection", 2, vmarg + 22);
        } else {
            g.drawString("Current:", 2, vmarg + 22);
            g.drawImage(images[currentElement], 96, vmarg + 4, this);
            g.drawRect(96, vmarg + 4, 48, 24);
        }

        /*Draw selection*/
        g.setColor(Color.yellow);
        int px = currentSelection.x * 48;
        int py = currentSelection.y * 24;
        g.drawRect(px, py, currentSelection.width * 48, currentSelection.height * 24);

    }

    public Dimension getPreferredSize() {
        Dimension r = new Dimension(20 * 48 + 1, 22 * 24 + 60);
        return r;
    }

    public int getCurrentCave() {
        return currentCave;
    }

    public void initImages() {
        this.setOpaque(true);
        this.setDoubleBuffered(true);

        ImageIcon i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elBlank.png"));
        images[CalominConstants.BLANK] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockFull.png"));
        images[CalominConstants.ROCK_FULL] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockTopLeft.png"));
        images[CalominConstants.ROCK_TL] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockTopRight.png"));
        images[CalominConstants.ROCK_TR] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockBottomLeft.png"));
        images[CalominConstants.ROCK_BL] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockBottomRight.png"));
        images[CalominConstants.ROCK_BR] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockUnstable.png"));
        images[CalominConstants.ROCK_UNSTABLE] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elLadder.png"));
        images[CalominConstants.LADDER] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elDeathBottomTop.png"));
        images[CalominConstants.DEATH_BOTTOM_TOP] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elDeathTopBottom.png"));
        images[CalominConstants.DEATH_TOP_BOTTOM] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elDiamond.png"));
        images[CalominConstants.DIAMOND] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elRockBroken.png"));
        images[CalominConstants.ROCK_BROKEN] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elStartPosition.png"));
        images[CalominConstants.START_POSITION] = i.getImage();

        i = new javax.swing.ImageIcon(getClass().getResource("/clmedit/elInvalid.png"));
        images[CalominConstants.INVALID] = i.getImage();

        this.addMouseMotionListener(mouseMotion);
        this.addMouseListener(mouseMotion);

    }

    public Rectangle getSelection() {
        return currentSelection;
    }

    private class DrawPanelMouseMotion extends MouseAdapter {

        private final Point selectionFirstPoint = new Point();

        public void mouseDragged(MouseEvent e) {

            int x = e.getX();
            int y = e.getY();
            int gx = x / 48;
            int gy = y / 24;

            if (gx > 19 || gy > 21 || gx < 0 || gy < 0) {
                return;
            }

            DrawPanel dp = (DrawPanel) e.getSource();

            if (dp.getSelectionMode() == true) {

                int newWidth = Math.abs(gx - selectionFirstPoint.x) + 1;
                int newHeight = Math.abs(gy - selectionFirstPoint.y) + 1;

                Rectangle rect = dp.getSelection();

                if (gx < selectionFirstPoint.x) {
                    rect.x = gx;
                } else {
                    rect.x = selectionFirstPoint.x;
                }
                if (gy < selectionFirstPoint.y) {
                    rect.y = gy;
                } else {
                    rect.y = selectionFirstPoint.y;
                }

                rect.width = newWidth;
                rect.height = newHeight;

                dp.repaint();

                return;
            }

            /*Normal element placement*/
            dp.placeCurrentElementAt(gx, gy);
        }

        public void mousePressed(MouseEvent e) {

            DrawPanel dp = (DrawPanel) e.getSource();
            if (dp.getSelectionMode() == false) {
                return;
            }

            int gx = e.getX() / 48;
            int gy = e.getY() / 24;

            if (gx > 19 || gy > 21 || gx < 0 || gy < 0) {
                return;
            }

            /*Begin selection*/
            selectionFirstPoint.x = gx;
            selectionFirstPoint.y = gy;

            /*Update selection to be painted*/
            dp.getSelection().x = gx;
            dp.getSelection().y = gy;
            dp.getSelection().height = 1;
            dp.getSelection().width = 1;

            dp.repaint();
        }

        public void mouseReleased(MouseEvent e) {
            DrawPanel dp = (DrawPanel) e.getSource();
            if (dp.getSelectionMode() == false) {
                return;
            }

        }

        public void mouseClicked(MouseEvent e) {

            int x = e.getX();
            int y = e.getY();
            int gx = x / 48;
            int gy = y / 24;

            if (gx > 19 || gy > 21 || gx < 0 || gy < 0) {
                return;
            }
            DrawPanel dp = (DrawPanel) e.getSource();

            /*If in selection mode*/
            if (dp.getSelectionMode() == true) {
                Rectangle rect = dp.getSelection();
                rect.x = gx;
                rect.y = gy;
                rect.width = 1;
                rect.height = 1;
                dp.repaint();
                return;
            }

            /*Element placement mode*/
            switch (e.getButton()) {
                case MouseEvent.BUTTON1:
                    dp.placeCurrentElementAt(gx, gy);
                    break;
            }
        }

    }

}
