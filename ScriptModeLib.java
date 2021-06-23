import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class ScriptModeLib {
    private final String target;
    private final BufferedReader reader;
    private String path;

    private final static String LS_COMMAND = "ls";
    private final static String CD_COMMAND = "cd";
    private final static String PWD_COMMAND = "pwd";
    private final static String EXIT_COMMAND = "exit";
    private final static String CP_COMMAND = "cp";
    private final static String HELP_COMMAND = "help";

    public ScriptModeLib(String target) {
        reader = new BufferedReader(
                new InputStreamReader(System.in));
        this.target = target;
        path = "";
    }

    public void run() throws IOException {
        switch (getPartition(getPath())) {
            case 0:
                System.out.println("FAT32 supported!");
                break;
            case 1:
                System.out.println("FAT32 not supported!");
                exit();
                return;
        }

        while (true) {
            System.out.print(getPath() + "> ");
            System.out.flush();
            String line = reader.readLine();
            String[] args;

            try {
                args = parseCommand(line);
            } catch (RuntimeException e) {
                System.out.println(e.getMessage());
                continue;
            }

            if (args.length < 1) {
                System.out.println(helpCommand());
                System.out.flush();
            } else {
                switch (args[0]) {
                    case (CP_COMMAND):
                        if (args.length < 3) {
                            System.out.println(helpCommand());
                            System.out.flush();

                            break;
                        }

                        switch (cpCommand(getPath(), args[1], args[2])) {
                            case 0:
                                System.out.println("Copied!");
                                break;
                            case 1:
                                System.out.println("Copy failed!");
                                System.out.println("Source dir/file not found!");
                                break;
                            case 2:
                                System.out.println("Copy failed!");
                                System.out.println("Destination dir/file not found!");
                                break;
                        }
                        System.out.flush();

                        break;
                    case (LS_COMMAND):
                        System.out.println(lsCommand(getPath()));
                        break;
                    case (CD_COMMAND):
                        if (args.length < 2) {
                            System.out.println(helpCommand());
                            System.out.flush();
                            break;
                        }

                        if (cdCommand(getPath(), args[1]) == 1) {
                            System.out.println("Cd failed!");
                            System.out.println("Dir not found!");
                        } else {
                            switch (args[1]) {
                                case "..":
                                    path = substringUntil(path, '/');
                                    break;
                                case ".":
                                    break;
                                default:
                                    path += "/" + args[1];
                                    break;
                            }
                        }
                        System.out.flush();

                        break;
                    case (PWD_COMMAND):
                        System.out.println(getPath());
                        break;
                    case (EXIT_COMMAND):
                        System.err.println("EXIT");
                        System.err.flush();
                        exit();
                        return;
                    case (HELP_COMMAND):
                    default:
                        System.out.println(helpCommand());
                        System.out.flush();

                        break;
                }
            }
        }
    }

    private String substringUntil(String path, char c) {
        for (int i = path.length() - 1; i >= 0; i--) {
            if (path.charAt(i) == c) {
                return path.substring(0, i);
            }
        }

        return "";
    }

    private String getPath() {
        return target + path;
    }

    private String[] parseCommand(String line) throws IllegalArgumentException {
        ArrayList<String> res = new ArrayList<>();
        line = line.trim();
        if (line.equals(LS_COMMAND)) {
            res.add(LS_COMMAND);
        } else if (line.equals(PWD_COMMAND)) {
            res.add(PWD_COMMAND);
        } else if (line.equals(EXIT_COMMAND)) {
            res.add(EXIT_COMMAND);
        } else if (line.startsWith(CD_COMMAND + " ")) {
            res.add(CD_COMMAND);
            line = line.substring(line.indexOf(" "));
            line = line.trim();

            if ((line.startsWith("'") || line.startsWith("\"")) && line.length() > 2) {
                char c = line.charAt(0);
                if (line.charAt(0) == line.charAt(line.length() - 1)) {
                    line = line.substring(1, line.length() - 1);

                    if (line.indexOf(c) == -1) {
                        res.add(line);
                    }
                }
            } else if (line.indexOf(' ') == -1 && line.indexOf('\t') == -1 && line.indexOf('\n') == -1) {
                res.add(line);
            }
        } else if (line.startsWith(CP_COMMAND + " ")) {
            res.add(CP_COMMAND);
            line = line.substring(line.indexOf(" "));
            line = line.trim();

            if (line.startsWith("'") || line.startsWith("\"")) {
                char c = line.charAt(0);
                line = line.substring(1);
                if (line.indexOf(c) == -1) {
                    return res.toArray(new String[0]);
                }

                res.add(line.substring(0, line.indexOf(c)));
                line = line.substring(line.indexOf(c));

                if (line.length() <= 1) {
                    return res.toArray(new String[0]);
                }
                line = line.substring(1);
            } else {
                for (int i = 0; i < line.length(); i++) {
                    if (line.charAt(i) == ' ' || line.charAt(i) == '\t' || line.charAt(i) == '\n') {
                        res.add(line.substring(0, i));
                        line = line.substring(i);
                        break;
                    }
                }
            }

            line = line.trim();

            if ((line.startsWith("'") || line.startsWith("\"")) && line.length() > 2) {
                char c = line.charAt(0);
                if (line.charAt(0) == line.charAt(line.length() - 1)) {
                    line = line.substring(1, line.length() - 1);

                    if (line.indexOf(c) == -1) {
                        res.add(line);
                    }
                }
            } else if (line.indexOf(' ') == -1 && line.indexOf('\t') == -1 && line.indexOf('\n') == -1) {
                res.add(line);
            }
        } else {
            res.add(HELP_COMMAND);
        }

        return res.toArray(new String[0]);
    }

    private void exit() throws IOException {
        reader.close();
    }

    static {
        try {
//            System.load("/home/ttaaa/gitlab.se.ifmo.ru/System-Software-Lab2/libScriptModeLib.so");
            System.loadLibrary("ScriptModeLib");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }

    public native int getPartition(String path);
    public native String lsCommand(String path);
    public native int cdCommand(String path, String to);
    public native int cpCommand(String path, String from, String to);
    public native String helpCommand();
}
