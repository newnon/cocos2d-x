package org.cocos2dx.lib;

import android.graphics.Color;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Looper;
import android.util.SparseArray;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.FrameLayout;
import android.util.DisplayMetrics;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewGroup.MarginLayoutParams;
import android.widget.ImageView;

import java.lang.reflect.Method;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

public class Cocos2dxWebViewHelper {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();
    private static Handler sHandler;
    private static Cocos2dxActivity sCocos2dxActivity;
    private static FrameLayout sLayout;

    private static SparseArray<Cocos2dxWebView> webViews;
    private static SparseArray<View> closeViews;
    private static int viewTag = 0;

    public Cocos2dxWebViewHelper(FrameLayout layout) {
        Cocos2dxWebViewHelper.sLayout = layout;
        Cocos2dxWebViewHelper.sHandler = new Handler(Looper.myLooper());

        Cocos2dxWebViewHelper.sCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        Cocos2dxWebViewHelper.webViews = new SparseArray<Cocos2dxWebView>();
        Cocos2dxWebViewHelper.closeViews = new SparseArray<View>();
    }

    private static native boolean shouldStartLoading(int index, String message);

    public static boolean _shouldStartLoading(int index, String message) {
        return !shouldStartLoading(index, message);
    }

    private static native void didFinishLoading(int index, String message);

    public static void _didFinishLoading(int index, String message) {
        didFinishLoading(index, message);
    }

    private static native void didFailLoading(int index, String message);

    public static void _didFailLoading(int index, String message) {
        didFailLoading(index, message);
    }

    private static native void onJsCallback(int index, String message);

    private static native void onCloseCallback(int index, String message);

    public static void _onJsCallback(int index, String message) {
        onJsCallback(index, message);
    }

    public static void _onCloseCallback(int index, String message) {
        onCloseCallback(index, message);
    }

    public static int createWebView(/*final Boolean enableCloseButton*/) {
        final int index = viewTag;
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = new Cocos2dxWebView(sCocos2dxActivity, index);
                FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                        FrameLayout.LayoutParams.WRAP_CONTENT,
                        FrameLayout.LayoutParams.WRAP_CONTENT);
                sLayout.addView(webView, lParams);
                webViews.put(index, webView);

                if (/*enableCloseButton*/true)
                {
                    DisplayMetrics metrics = sCocos2dxActivity.getResources().getDisplayMetrics();
                    MarginLayoutParams params = new MarginLayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
                    params.setMargins(0, 0, metrics.widthPixels - 400, metrics.heightPixels - 200);
                    ImageView closeButton = new ImageView(sCocos2dxActivity);
                    closeButton.setImageResource(R.drawable.btn_back_arrow);
                    closeButton.setLayoutParams(params);
                    closeButton.setBackgroundColor(0x00000000);
                    closeButton.setOnClickListener(new View.OnClickListener()
                    {
                        @Override
                        public void onClick(View v)
                        {
                            sCocos2dxActivity.runOnGLThread(new Runnable()
                            {
                                @Override
                                public void run()
                                {
                                    _onCloseCallback(index, "");
                                }
                            });
                        }
                    });
                    sLayout.addView(closeButton);
                    closeViews.put(index, closeButton);
                }
            }
        });
        return viewTag++;
    }

    public static void removeWebView(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webViews.remove(index);
                    sLayout.removeView(webView);
                }
                View closeView = closeViews.get(index);
                if (closeView != null) {
                    closeViews.remove(index);
                    sLayout.removeView(closeView);
                }
            }
        });
    }

    public static void setVisible(final int index, final boolean visible) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setVisibility(visible ? View.VISIBLE : View.GONE);
                }
                View closeView = closeViews.get(index);
                if (closeView != null) {
                    closeView.setVisibility(visible ? View.VISIBLE : View.GONE);
                }
            }
        });
    }

    public static void setBackgroundTransparent(final int index) {
        if(android.os.Build.VERSION.SDK_INT >10) {
            sCocos2dxActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebView webView = webViews.get(index);
                    if (webView != null) {
                        webView.setBackgroundColor(Color.TRANSPARENT);
                        try {
                            Method method = webView.getClass().getMethod("setLayerType",int.class,Paint.class);
                            method.invoke(webView,WebView.LAYER_TYPE_SOFTWARE,null);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
        }
    }

    public static void setOpacityWebView(final int index, final float opacity) {
        if(android.os.Build.VERSION.SDK_INT >10){
            sCocos2dxActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebView webView = webViews.get(index);
                    if (webView != null) {
                        try {
                            Method method = webView.getClass().getMethod("setAlpha",float.class);
                            method.invoke(webView,opacity);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
        }
    }


    public static float getOpacityWebView(final int index) {
        if(android.os.Build.VERSION.SDK_INT >10){
            FutureTask<Float> futureResult = new FutureTask<Float>(new Callable<Float>() {
            @Override
            public Float call() throws Exception {
                float opacity=0.f;
                Cocos2dxWebView webView = webViews.get(index);
                Object valueToReturn=null;
                if (webView != null) {
                    try {
                        Method method = webView.getClass().getMethod("getAlpha");
                        valueToReturn = method.invoke(webView);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                return (Float) valueToReturn;
            }
            });
            sCocos2dxActivity.runOnUiThread(futureResult);
            try {
                return futureResult.get();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return 1;
    }

    public static void setWebViewRect(final int index, final int left, final int top, final int maxWidth, final int maxHeight) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setWebViewRect(left, top, maxWidth, maxHeight);
                }
            }
        });
    }

    public static void setLocalFiles(final int index, final Map<String, String> localFiles) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setLocalFiles(localFiles);
                }
            }
        });
    }

    public static void setJavascriptInterfaceScheme(final int index, final String scheme) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setJavascriptInterfaceScheme(scheme);
                }
            }
        });
    }

    public static void loadData(final int index, final String data, final String mimeType, final String encoding, final String baseURL) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseURL, data, mimeType, encoding, null);
                }
            }
        });
    }

    public static void loadHTMLString(final int index, final String data, final String baseUrl) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseUrl, data, null, null, null);
                }
            }
        });
    }

    public static void loadUrl(final int index, final String url, final boolean cleanCachedData) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.getSettings().setCacheMode(cleanCachedData ? WebSettings.LOAD_NO_CACHE
                                                                       : WebSettings.LOAD_DEFAULT);
                    webView.loadUrl(url);
                }
            }
        });
    }

    public static void loadFile(final int index, final String filePath) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl(filePath);
                }
            }
        });
    }

    public static void stopLoading(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.stopLoading();
                }
            }
        });

    }

    public static void reload(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.reload();
                }
            }
        });
    }

    public static <T> T callInMainThread(Callable<T> call) throws ExecutionException, InterruptedException {
        FutureTask<T> task = new FutureTask<T>(call);
        sHandler.post(task);
        return task.get();
    }

    public static boolean canGoBack(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                Cocos2dxWebView webView = webViews.get(index);
                return webView != null && webView.canGoBack();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    public static boolean canGoForward(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                Cocos2dxWebView webView = webViews.get(index);
                return webView != null && webView.canGoForward();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    public static void goBack(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goBack();
                }
            }
        });
    }

    public static void goForward(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goForward();
                }
            }
        });
    }

    public static void evaluateJS(final int index, final String js) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl("javascript:" + js);
                }
            }
        });
    }

    public static void setScalesPageToFit(final int index, final boolean scalesPageToFit) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setScalesPageToFit(scalesPageToFit);
                }
            }
        });
    }
}
