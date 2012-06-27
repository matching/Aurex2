package jp.ruputer.AndroAurex;

import android.content.Context;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class AndroAurexSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    public AndroAurexSurfaceView(Context context) {
        super(context);

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    	Log.d("Androaurex", "surfaceChanged called.");
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.d("Androaurex", "surfaceCreated called.");
   	
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d("Androaurex", "surfaceDestroyed");
    }
    
}
