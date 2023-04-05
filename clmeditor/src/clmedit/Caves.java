package clmedit;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;

class Caves {

    /**
     * Load caves from a file
     *
     * @param absolutePath File
     * @return Number of caves loaded
     */
    public static final int loadCaves(String absolutePath, int[][][] caveData) throws Exception {
        final int oneCaveSize = (22 * 20 / 2) + 2;

        RandomAccessFile raf = new RandomAccessFile(absolutePath, "r");
        long l = raf.length();
        raf.close();

        /*Check if the length is correct*/
        if (l == 0) {
            throw new NumberFormatException("Input file is empty");
        }
        if (l > 32 * oneCaveSize) {
            throw new NumberFormatException("Input file is too long");
        }
        if ((l % oneCaveSize) != 0) {
            throw new NumberFormatException("Input file does not contain caves. File size not divisible by cave size");
        }

        int maxIndex = ((int) l / oneCaveSize) - 1;

        FileInputStream fis = new FileInputStream(absolutePath);
        for (int caveIndex = 0; caveIndex <= maxIndex; caveIndex++) {

            /*Determine miner position*/
            int posY = fis.read();
            int posX = fis.read();

            /*Read cave data. 4 bits per cave element*/
            for (int y = 0; y < 22; y++) {
                for (int x = 0; x < 20; x += 2) {

                    int twoElems = fis.read();
                    caveData[caveIndex][y][x] = ((twoElems >> 4) & 0xF);
                    caveData[caveIndex][y][x + 1] = (twoElems & 0xF);
                }
            }
            caveData[caveIndex][posY][posX] = CalominConstants.START_POSITION;
        }

        fis.close();

        return maxIndex;
    }

    /**
     * Save caves to a file
     *
     * @param absolutePath File
     */
    public static final void saveCaves(String absolutePath, int[][][] caveData, int maxIndex) throws Exception {
        /*Copy of data*/
        int[][][] saveCaveData = new int[32][22][20];

        /*Copy and check for invalid elements*/
        for (int caveIndex = 0; caveIndex <= maxIndex; caveIndex++) {

            int treasureCount = 0;
            int startPositionCount = 0;

            for (int y = 0; y < 22; y++) {
                for (int x = 0; x < 20; x++) {

                    int element = caveData[caveIndex][y][x];

                    /*Check for invalid elements*/
                    if (element == CalominConstants.INVALID) {
                        throw new NumberFormatException("Cannot save caves. Cave has invalid element(s). Cave index: " + Integer.toString(caveIndex));
                    }

                    /*Check if there is a starting position*/
                    if (element == CalominConstants.START_POSITION) {
                        startPositionCount++;
                    }

                    /*Check for treasure*/
                    if (element == CalominConstants.DIAMOND) {
                        treasureCount++;
                    }

                    saveCaveData[caveIndex][y][x] = caveData[caveIndex][y][x];

                }
            }

            /*Verify if everything is OK with the cave*/
            if (startPositionCount != 1) {
                throw new NumberFormatException("Cannot save caves. Cave has no or multiple starting positions. Cave index: " + caveIndex);
            }
            if (treasureCount == 0) {
                throw new NumberFormatException("Cannot save caves. Cave has no treasure. Cave index: " + caveIndex);
            }
        }

        FileOutputStream fos = new FileOutputStream(absolutePath);

        for (int caveIndex = 0; caveIndex <= maxIndex; caveIndex++) {

            /*Miner position*/
            int posY = 0;
            int posX = 0;

            /*Determine miner position*/
            for (int y = 0; y < 22; y++) {
                for (int x = 0; x < 20; x++) {
                    if (saveCaveData[caveIndex][y][x] == CalominConstants.START_POSITION) {
                        posY = y;
                        posX = x;
                        saveCaveData[caveIndex][y][x] = CalominConstants.BLANK;
                        break;
                    }
                }
            }


            /*Write miner position*/
            fos.write(posY);
            fos.write(posX);

            /*Write cave data. 4 bits per cave element. Do not allow to save invalid element*/
            for (int y = 0; y < 22; y++) {
                for (int x = 0; x < 20; x += 2) {

                    int hiElem = saveCaveData[caveIndex][y][x];
                    int loElem = saveCaveData[caveIndex][y][x + 1];
                    fos.write(((hiElem << 4) | loElem));

                }
            }
        }

        fos.close();
    }

    /**
     * Import old "Caverns of the lost miner" caves
     *
     * @param absolutePath File
     * @return Number of caves imported
     */
    public static final int importOldCaves(String absolutePath, int[][][] caveData) throws Exception {
        final int oneCaveSize = 22 * 20;

        int[] translateElem = {
            CalominConstants.ROCK_FULL,
            CalominConstants.DEATH_BOTTOM_TOP,
            CalominConstants.ROCK_BR,
            CalominConstants.ROCK_BL,
            CalominConstants.ROCK_TL,
            CalominConstants.ROCK_TR,
            CalominConstants.DEATH_TOP_BOTTOM,
            CalominConstants.LADDER,
            CalominConstants.ROCK_BROKEN,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.ROCK_UNSTABLE,
            CalominConstants.START_POSITION,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.DIAMOND,
            CalominConstants.DIAMOND,
            CalominConstants.DIAMOND,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.INVALID,
            CalominConstants.BLANK
        };

        RandomAccessFile raf = new RandomAccessFile(absolutePath, "r");
        long l = raf.length();
        raf.close();

        /*Check if the length is correct*/
        if (l == 0) {
            throw new NumberFormatException("Input file is empty");
        }
        if (l > 32 * oneCaveSize) {
            throw new NumberFormatException("Input file is too long");
        }
        if ((l % oneCaveSize) != 0) {
            throw new NumberFormatException("Input file does not contain old caves. File size not divisible by cave size");
        }

        int maxIndex = ((int) l / oneCaveSize) - 1;

        FileInputStream fis = new FileInputStream(absolutePath);
        for (int caveIndex = 0; caveIndex <= maxIndex; caveIndex++) {

            /*Get data*/
            for (int x = 0; x < 20; x++) {
                for (int y = 0; y < 22; y++) {
                    int oldElem = fis.read();
                    caveData[caveIndex][y][x] = translateElem[oldElem - 1];
                }
            }
        }

        fis.close();
        return maxIndex;
    }

}
