package me.simple.lib.utils;


import android.os.Build;
import android.os.Handler;
import android.os.Looper;

import androidx.annotation.RequiresApi;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.List;

import me.simple.lib.BuildConfig;

/**
 * <pre>
 *     author: Blankj
 *     blog  : http://blankj.com
 *     time  : 2016/08/07
 *     desc  : utils about shell
 * </pre>
 */
public final class ShellUtils {

    private static final Handler UTIL_HANDLER = new Handler(Looper.getMainLooper());
    public static boolean isActivice() {
       // return BuildConfig.DEBUG;
        return false;
    }
    public static boolean isActivice2() {
        // return BuildConfig.DEBUG;
        return false;
    }

    public abstract static class Task<Result> implements Runnable {

        private static final int NEW         = 0;
        private static final int COMPLETING  = 1;
        private static final int CANCELLED   = 2;
        private static final int EXCEPTIONAL = 3;

        private volatile int state = NEW;

        abstract Result doInBackground();

        private final Callback<Result> mCallback;

        public Task(final Callback<Result> callback) {
            mCallback = callback;
        }

        @Override
        public void run() {
            try {
                final Result t = doInBackground();
                if (state != NEW) return;
                state = COMPLETING;
                UTIL_HANDLER.post(() -> mCallback.onCall(t));
            } catch (Throwable th) {
                if (state != NEW) return;
                state = EXCEPTIONAL;
            }
        }

        public void cancel() {
            state = CANCELLED;
        }

        public boolean isDone() {
            return state != NEW;
        }

        public boolean isCanceled() {
            return state == CANCELLED;
        }
    }

    public interface Callback<T> {
        void onCall(T data);
    }


    private ShellUtils() {
        throw new UnsupportedOperationException("u can't instantiate me...");
    }

    /**
     * Execute the command asynchronously.
     *
     * @param command  The command.
     * @param isRooted True to use root, false otherwise.
     * @param callback The callback.
     * @return the task
     */
    public static Task<CommandResult> execCmdAsync(final String command,
                                                   final boolean isRooted,
                                                   final Callback<CommandResult> callback) {
        return execCmdAsync(new String[]{command}, isRooted, true, callback);
    }

    /**
     * Execute the command asynchronously.
     *
     * @param commands The commands.
     * @param isRooted True to use root, false otherwise.
     * @param callback The callback.
     * @return the task
     */
    public static Task<CommandResult> execCmdAsync(final List<String> commands,
                                                   final boolean isRooted,
                                                   final Callback<CommandResult> callback) {
        return execCmdAsync(commands == null ? null : commands.toArray(new String[]{}), isRooted, true, callback);
    }

    /**
     * Execute the command asynchronously.
     *
     * @param commands The commands.
     * @param isRooted True to use root, false otherwise.
     * @param callback The callback.
     * @return the task
     */
    public static Task<CommandResult> execCmdAsync(final String[] commands,
                                                   final boolean isRooted,
                                                   final Callback<CommandResult> callback) {
        return execCmdAsync(commands, isRooted, true, callback);
    }

    /**
     * Execute the command asynchronously.
     *
     * @param command         The command.
     * @param isRooted        True to use root, false otherwise.
     * @param isNeedResultMsg True to return the message of result, false otherwise.
     * @param callback        The callback.
     * @return the task
     */
    public static Task<CommandResult> execCmdAsync(final String command,
                                                   final boolean isRooted,
                                                   final boolean isNeedResultMsg,
                                                   final Callback<CommandResult> callback) {
        return execCmdAsync(new String[]{command}, isRooted, isNeedResultMsg, callback);
    }

    /**
     * Execute the command asynchronously.
     *
     * @param commands        The commands.
     * @param isRooted        True to use root, false otherwise.
     * @param isNeedResultMsg True to return the message of result, false otherwise.
     * @param callback        The callback.
     * @return the task
     */
    public static Task<CommandResult> execCmdAsync(final List<String> commands,
                                                   final boolean isRooted,
                                                   final boolean isNeedResultMsg,
                                                   final Callback<CommandResult> callback) {
        return execCmdAsync(commands == null ? null : commands.toArray(new String[]{}),
                            isRooted,
                            isNeedResultMsg,
                            callback);
    }

    /**
     * Execute the command asynchronously.
     *
     * @param commands        The commands.
     * @param isRooted        True to use root, false otherwise.
     * @param isNeedResultMsg True to return the message of result, false otherwise.
     * @param callback        The callback.
     * @return the task
     */
    public static Task<CommandResult> execCmdAsync(final String[] commands,
                                                   final boolean isRooted,
                                                   final boolean isNeedResultMsg,
                                                   final Callback<CommandResult> callback) {return null;
	}

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static boolean isRootGiven() {
        try {
            Runtime.getRuntime().exec("su", null, null);
            String readLine = execCmd("id", true, true).successMsg;
            return readLine != null && readLine.toLowerCase().contains("uid=0");
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execCmd(final String command, final boolean isRooted) {
        return execCmd(new String[]{command}, isRooted, true);
    }

    /**
     * Execute the command.
     *
     * @param commands The commands.
     * @param isRooted True to use root, false otherwise.
     * @return the single {@link CommandResult} instance
     */
    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execCmd(final List<String> commands, final boolean isRooted) {
        return execCmd(commands == null ? null : commands.toArray(new String[]{}), isRooted, true);
    }

    /**
     * Execute the command.
     *
     * @param commands The commands.
     * @param isRooted True to use root, false otherwise.
     * @return the single {@link CommandResult} instance
     */
    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execCmd(final String[] commands, final boolean isRooted) {
        return execCmd(commands, isRooted, true);
    }

    /**
     * Execute the command.
     *
     * @param command         The command.
     * @param isRooted        True to use root, false otherwise.
     * @param isNeedResultMsg True to return the message of result, false otherwise.
     * @return the single {@link CommandResult} instance
     */
    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execCmd(final String command,
                                        final boolean isRooted,
                                        final boolean isNeedResultMsg) {
        return execCmd(new String[]{command}, isRooted, isNeedResultMsg);
    }

    /**
     * Execute the command.
     *
     * @param commands        The commands.
     * @param isRooted        True to use root, false otherwise.
     * @param isNeedResultMsg True to return the message of result, false otherwise.
     * @return the single {@link CommandResult} instance
     */
    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execCmd(final List<String> commands,
                                        final boolean isRooted,
                                        final boolean isNeedResultMsg) {
        return execCmd(commands == null ? null : commands.toArray(new String[]{}),
                       isRooted,
                       isNeedResultMsg);
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execCmd(final String[] commands,
                                        final boolean isRooted,
                                        final boolean isNeedResultMsg) {
        int result = -1;
        if (commands == null || commands.length == 0) {
            return new CommandResult(result, "", "");
        }
        //Log.d("ShellUtils", Arrays.toString(commands));
        Process process = null;
        BufferedReader successResult = null;
        BufferedReader errorResult = null;
        StringBuilder successMsg = null;
        StringBuilder errorMsg = null;
        DataOutputStream os = null;
        try {
            process = Runtime.getRuntime().exec(isRooted ? "su -mm" : "sh");
            os = new DataOutputStream(process.getOutputStream());
            for (String command : commands) {
                if (command == null) continue;
                //os.write(command.getBytes());
                os.writeBytes(command + "\n");
                os.flush();
            }
            os.writeBytes("exit\n");
            os.flush();
            result = process.waitFor();
            if (isNeedResultMsg) {
                successMsg = new StringBuilder();
                errorMsg = new StringBuilder();
                successResult = new BufferedReader(
                    new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8)
                );
                errorResult = new BufferedReader(
                    new InputStreamReader(process.getErrorStream(), StandardCharsets.UTF_8)
                );
                String line;
                if ((line = successResult.readLine()) != null) {
                    successMsg.append(line);
                    while ((line = successResult.readLine()) != null) {
                        successMsg.append("\n").append(line);
                    }
                }
                if ((line = errorResult.readLine()) != null) {
                    errorMsg.append(line);
                    while ((line = errorResult.readLine()) != null) {
                        errorMsg.append("\n").append(line);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                if (successResult != null) {
                    successResult.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                if (errorResult != null) {
                    errorResult.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (process != null) {
                process.destroy();
            }
        }
        return new CommandResult(
                result,
                successMsg == null ? "" : successMsg.toString(),
                errorMsg == null ? "" : errorMsg.toString()
        );
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static boolean FileIsExists(String path) {
        String cmd = "if [ -f \"" + path + "\" ];then\necho \"YES\"\nelse\necho \"NO\"\nfi";
        String temp = execCmd(cmd, true, true).successMsg;
        return temp.equals("YES");
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static CommandResult execadb(final String[] commands, final boolean isNeedResultMsg) {
        int result = -1;
        if (commands == null || commands.length == 0) {
            return new CommandResult(result, "", "");
        }
        Process process = null;
        BufferedReader successResult = null;
        BufferedReader errorResult = null;
        StringBuilder successMsg = null;
        StringBuilder errorMsg = null;
        DataOutputStream os = null;
        try {
            process = Runtime.getRuntime().exec("su 2000");
            os = new DataOutputStream(process.getOutputStream());
            for (String command : commands) {
                if (command == null) continue;
                //os.write(command.getBytes());
                os.writeBytes(command + "\n");
                os.flush();
            }
            os.writeBytes("exit\n");
            os.flush();
            result = process.waitFor();
            if (isNeedResultMsg) {
                successMsg = new StringBuilder();
                errorMsg = new StringBuilder();
                successResult = new BufferedReader(
                        new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8)
                );
                errorResult = new BufferedReader(
                        new InputStreamReader(process.getErrorStream(), StandardCharsets.UTF_8)
                );
                String line;
                if ((line = successResult.readLine()) != null) {
                    successMsg.append(line);
                    while ((line = successResult.readLine()) != null) {
                        successMsg.append("\n").append(line);
                    }
                }
                if ((line = errorResult.readLine()) != null) {
                    errorMsg.append(line);
                    while ((line = errorResult.readLine()) != null) {
                        errorMsg.append("\n").append(line);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                if (successResult != null) {
                    successResult.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                if (errorResult != null) {
                    errorResult.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (process != null) {
                process.destroy();
            }
        }
        return new CommandResult(
                result,
                successMsg == null ? "" : successMsg.toString(),
                errorMsg == null ? "" : errorMsg.toString()
        );
    }
    /**
     * The result of command.
     */
    public static class CommandResult {
        public int    result;
        public String successMsg;
        public String errorMsg;

        public CommandResult(final int result, final String successMsg, final String errorMsg) {
            this.result = result;
            this.successMsg = successMsg;
            this.errorMsg = errorMsg;
        }

        @Override
        public String toString() {
            return "result: " + result + "\n" +
                "successMsg: " + successMsg + "\n" +
                "errorMsg: " + errorMsg;
        }
    }


}
