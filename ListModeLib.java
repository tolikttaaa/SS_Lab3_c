public class ListModeLib {
    static {
    	try {
//            System.load("/home/ttaaa/gitlab.se.ifmo.ru/System-Software-Lab2/libListModeLib.so");
            System.loadLibrary("ListModeLib");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }

    public native String runListMode();
}
