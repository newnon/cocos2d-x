package org.cocos2dx.lib;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.webkit.MimeTypeMap;
import android.webkit.WebChromeClient;
import android.webkit.WebResourceResponse;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

import java.io.IOException;
import java.lang.reflect.Method;
import java.net.URI;
import java.util.Map;
import java.util.concurrent.CountDownLatch;

class ShouldStartLoadingWorker implements Runnable {
    private CountDownLatch mLatch;
    private boolean[] mResult;
    private final int mViewTag;
    private final String mUrlString;

    ShouldStartLoadingWorker(CountDownLatch latch, boolean[] result, int viewTag, String urlString) {
        this.mLatch = latch;
        this.mResult = result;
        this.mViewTag = viewTag;
        this.mUrlString = urlString;
    }

    @Override
    public void run() {
        this.mResult[0] = Cocos2dxWebViewHelper._shouldStartLoading(mViewTag, mUrlString);
        this.mLatch.countDown(); // notify that result is ready
    }
}

public class Cocos2dxWebView extends WebView {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();

    private int mViewTag;
    private String mJSScheme;
    private Context context;
    private Cocos2dxWebViewClient webViewClient;

    public Cocos2dxWebView(Context context) {
        this(context, -1);
    }

    @SuppressLint("SetJavaScriptEnabled")
    public Cocos2dxWebView(Context context, int viewTag) {
        super(context);
        this.context = context;
        this.mViewTag = viewTag;
        this.mJSScheme = "";

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);

        this.getSettings().setSupportZoom(false);

        this.getSettings().setDomStorageEnabled(true);
        this.getSettings().setJavaScriptEnabled(true);
        this.getSettings().setAllowFileAccessFromFileURLs(true);
        this.getSettings().setAllowUniversalAccessFromFileURLs(true);
        Log.d(TAG, "DEBUG_WEBVIEW userAgent before = " + this.getSettings().getUserAgentString());
        this.getSettings().setUserAgentString(this.getSettings().getUserAgentString().replace("Android", ""));
        Log.d(TAG, "DEBUG_WEBVIEW userAgent after = " + this.getSettings().getUserAgentString());

        // `searchBoxJavaBridge_` has big security risk. http://jvn.jp/en/jp/JVN53768697
        try {
            Method method = this.getClass().getMethod("removeJavascriptInterface", new Class[]{String.class});
            method.invoke(this, "searchBoxJavaBridge_");
        } catch (Exception e) {
            Log.d(TAG, "This API level do not support `removeJavascriptInterface`");
        }

        this.webViewClient = new Cocos2dxWebViewClient();
        this.setWebViewClient(webViewClient);
        this.setWebChromeClient(new WebChromeClient());
    }

    public void setLocalFiles(Map<String, String> localFiles)
    {
        this.webViewClient.setLocalFiles(localFiles);
    }

    public void setJavascriptInterfaceScheme(String scheme) {
        this.mJSScheme = scheme != null ? scheme : "";
    }

    public void setScalesPageToFit(boolean scalesPageToFit) {
        this.getSettings().setSupportZoom(scalesPageToFit);
    }

    class Cocos2dxWebViewClient extends WebViewClient {

        private Map<String, String> localFiles = null;

        public void setLocalFiles(Map<String, String> localFiles)
        {
            this.localFiles = localFiles;
        }

        @Override
        public WebResourceResponse shouldInterceptRequest(WebView view, String url) {
            if(localFiles == null)
                return null;

            Uri uri = Uri.parse(url);
            String server = uri.getAuthority();
            String path = uri.getPath();
            String localPath  = localFiles.get(server + path);
            if(localPath != null)
            {
                String extension = MimeTypeMap.getFileExtensionFromUrl(url);
                try {
                    return new WebResourceResponse(MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension), "UTF-8", context.getAssets().open(localPath));
                }
                catch (IOException e) {
                    Log.e(TAG, "failed to load local file", e);
                    return null;
                }
            }
            else
            {
                return null;
            }
        }

        @Override
        public boolean shouldOverrideUrlLoading(WebView view, final String urlString) {
            Cocos2dxActivity activity = (Cocos2dxActivity)getContext();

            try {
                URI uri = URI.create(urlString);
                if (uri != null && uri.getScheme().equals(mJSScheme)) {
                    activity.runOnGLThread(new Runnable() {
                        @Override
                        public void run() {
                            Cocos2dxWebViewHelper._onJsCallback(mViewTag, urlString);
                        }
                    });
                    return true;
                }
            } catch (Exception e) {
                Log.d(TAG, "Failed to create URI from url");
            }

            boolean[] result = new boolean[] { true };
            CountDownLatch latch = new CountDownLatch(1);

            // run worker on cocos thread
            activity.runOnGLThread(new ShouldStartLoadingWorker(latch, result, mViewTag, urlString));

            // wait for result from cocos thread
            try {
                latch.await();
            } catch (InterruptedException ex) {
                Log.d(TAG, "'shouldOverrideUrlLoading' failed");
            }

            return result[0];
        }

        @Override
        public void onPageFinished(WebView view, final String url) {
            super.onPageFinished(view, url);
            Cocos2dxActivity activity = (Cocos2dxActivity)getContext();
            activity.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebViewHelper._didFinishLoading(mViewTag, url);
                }
            });
        }

        @Override
        public void onReceivedError(WebView view, int errorCode, String description, final String failingUrl) {
            super.onReceivedError(view, errorCode, description, failingUrl);
            Cocos2dxActivity activity = (Cocos2dxActivity)getContext();
            activity.runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebViewHelper._didFailLoading(mViewTag, failingUrl);
                }
            });
        }
    }

    public void setWebViewRect(int left, int top, int maxWidth, int maxHeight) {
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.leftMargin = left;
        layoutParams.topMargin = top;
        layoutParams.width = maxHeight;
        layoutParams.height = maxWidth;
        layoutParams.gravity = Gravity.TOP | Gravity.LEFT;
        this.setLayoutParams(layoutParams);
    }
}
