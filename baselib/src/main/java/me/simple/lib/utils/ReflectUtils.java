package me.simple.lib.utils;

import com.google.gson.internal.$Gson$Types;


import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

import de.robv.android.xposed.XC_MethodHook;
import me.simple.lib.exception.NotClassException;

public class ReflectUtils {

    private ReflectUtils() {
    }

    public static Type getTypeArguments(Class<?> subclass, boolean isMainClass) throws NotClassException {
        return getTypeArguments(subclass, isMainClass, 0, 0);
    }

    /**
     *
     * @param subclass 原始的用来构造回调的类型，如果是主类，其泛型在一个类的上面，所以要查找其getGenericSuperclass，
     *                 如果是一个implent接口生成的匿名类，则查找的是其接口。绝大部分的情况下都是实现的匿名接口。
     * @param isMainClass
     * @param wrapLevel 这个用来处理包裹多个的情况，需要多次解析，这个用来出来包裹多个参数的情况 AbstractWrapData<T1, T2>处理的是泛型的泛型，真正需要自动反射的是其T1,T2参数。所以wraplevel = 1
     * @param finalTypeIndex 包裹多个参数时，也需要上层告诉底层到低取值哪一个进行反射处理。
     * @return
     * @throws NotClassException
     */
    public static Type getTypeArguments(Class<?> subclass, boolean isMainClass, int wrapLevel, int  finalTypeIndex) throws NotClassException {
        //得到带有泛型的类
        Type generic;
        if (isMainClass) {
            generic = subclass.getGenericSuperclass();
        } else {
            generic = subclass.getGenericInterfaces()[0];
        }
       // LogUtils.D("GsonFunctionMap", "generic:" + generic +  " wrapLevel: " + wrapLevel);
        if (generic instanceof Class) {
            throw new NotClassException("Missing type parameter.");
        }
        //取出当前类的泛型
        ParameterizedType parameter = (ParameterizedType) generic;
        Type resultTyep;
        if (wrapLevel == 0) {
           resultTyep =  $Gson$Types.canonicalize(parameter.getActualTypeArguments()[finalTypeIndex]);
        } else {
            resultTyep = getTypeArgumentsWithType(parameter.getActualTypeArguments()[0],  --wrapLevel, finalTypeIndex);
        }
        if (null != resultTyep) {
           // LogUtils.D("GsonFunctionMap", "resultTyep:" +  resultTyep.toString() + " "+ (resultTyep instanceof ParameterizedType));
        }
        return resultTyep;
    }

    public static Type getTypeArgumentsWithType(Type type, int wrapLevel, int  finalTypeIndex) throws NotClassException {
        //得到带有泛型的类
        LogUtils.D("GsonFunctionMap", "getTypeArgumentsWithType generic:" + type + " wrapLevel: " + wrapLevel);
        if (type instanceof Class) {
            throw new NotClassException("Missing type parameter.");
        }
        //取出当前类的泛型
        ParameterizedType parameter = (ParameterizedType) type;
        Type resultTyep;
        if (wrapLevel == 0) {
            resultTyep =  $Gson$Types.canonicalize(parameter.getActualTypeArguments()[finalTypeIndex]);
        } else {
            resultTyep = getTypeArgumentsWithType(parameter.getActualTypeArguments()[0], --wrapLevel, finalTypeIndex);
        }
        if (null != resultTyep) {
            LogUtils.D("GsonFunctionMap", "resultTyep:" +  resultTyep.toString() + " "+ (resultTyep instanceof ParameterizedType));
        }
        return resultTyep;
    }

    public static Object rejectFind(Object newinstance, String fieldType, String fieldName) {
        Class classparam = newinstance.getClass();
        Field[] fields = classparam.getFields();
        Object result = null;
        for (int i = 0; i < fields.length; i++) {
            Field field = fields[i];
            field.setAccessible(true);
            if (field.getType().getName().equals(fieldType) && field.getName().endsWith(fieldName)) {
                try {
                    result = field.get(newinstance);
                    break;
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                }
            }
        }

        Field[] declaredFields = classparam.getDeclaredFields();
        for (int i = 0; i < declaredFields.length; i++) {
            Field field = declaredFields[i];
            field.setAccessible(true);
            if (field.getType().getName().equals(fieldType) && field.getName().endsWith(fieldName)) {
                try {
                    result = field.get(newinstance);
                    break;
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    public static Object getThisObject(XC_MethodHook.MethodHookParam methodHookParam) {
        if (null == methodHookParam) {
            return null;
        }
        return rejectFind(methodHookParam, "java.lang.Object", "thisObject");
    }

    public static Method findMethod(Class classToFind, String methodName) {
        Method[] methods = classToFind.getMethods();
        for (int i = 0; i < methods.length; i++) {
            Method method = methods[i];
            if (method.getName().equals(methodName)) {
                return method;
            }
        }

        Method[] declaredMethods = classToFind.getDeclaredMethods();
        for (int i = 0; i < declaredMethods.length; i++) {
            Method method = declaredMethods[i];
            if (method.getName().equals(methodName)) {
                return method;
            }
        }
        return null;

    }

    public static Method findMethod(Class classToFind, String methodName, int paramsLenght) {
        Method[] methods = classToFind.getMethods();
        for (int i = 0; i < methods.length; i++) {
            Method method = methods[i];
            if (method.getName().equals(methodName) && method.getParameterTypes().length == paramsLenght) {
                return method;
            }
        }

        Method[] declaredMethods = classToFind.getDeclaredMethods();
        for (int i = 0; i < declaredMethods.length; i++) {
            Method method = declaredMethods[i];
            if (method.getName().equals(methodName) && method.getParameterTypes().length == paramsLenght) {
                return method;
            }
        }
        return null;

    }

    public static Field findFiled(Class classToFind, String fieldName) {
        if (classToFind == null) {
            return null;
        }
        Field[] fields = classToFind.getFields();
        for (int i = 0; i < fields.length; i++) {
            Field field = fields[i];
            if (field.getName().equals(fieldName)) {
                return field;
            }
        }

        Field[] declaredFields = classToFind.getDeclaredFields();
        for (int i = 0; i < declaredFields.length; i++) {
            Field field = declaredFields[i];
            if (field.getName().equals(fieldName)) {
                return field;
            }
        }
        return findFiled(classToFind.getSuperclass(), fieldName);
    }
}
