import java.io.IOException;

public class Main {
    private static final String SCRIPT_MODE = "script";
    private static final String MOUNTS_LIST_MODE = "list";

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.printf("Please, specify program mode (%s/%s)\n", SCRIPT_MODE, MOUNTS_LIST_MODE);
        } else {
            switch (args[0]) {
                case SCRIPT_MODE:
                    if (args.length < 2) {
                        System.out.println("You have to specify target device with FAT32 fs");
                        System.out.println("usage: ./lab1 script sda1");
                        return;
                    }

                    System.out.println("Starting program in script mode...\n");
                    try {
                        new ScriptModeLib(args[1]).run();
                    } catch (IOException e) {
                        e.getMessage();
                    }
                    break;
                case MOUNTS_LIST_MODE:
                    System.out.println("Starting program in mounts mode...");
                    System.out.println(new ListModeLib().runListMode());
                    break;
                default:
                    System.out.println("Unknown mode, terminating program!");
                    return;
            }

            System.out.println("Finish working!");
        }
    }
}
