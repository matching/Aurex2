package jp.ruputer.AndroAurex;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;

public class AndroAurex extends Activity {

    AndroAurexSurfaceView sv;
    JniIF jniif;
    
	static {
		// ライブラリをロード
		System.loadLibrary("jniaurex");
	}

	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		setContentView(R.layout.main);
		
		SurfaceView displayView = (SurfaceView) findViewById(R.id.displayView);

		Log.v("androaurex", "display view " + displayView );

		
    	sv = new AndroAurexSurfaceView( this );
//        setContentView( sv );
    	
		displayView.getHolder().addCallback( sv );
//        displayView.getHolder().setFixedSize(displayView.getWidth(), displayView.getHeight());    

        int w, h;
        int bairitu = 1;
        
        WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        Display display = wm.getDefaultDisplay();
         
        
        w = display.getWidth();
        h = display.getHeight();
		Log.v("androaurex", "original w, h  = " + w + ", " + h );

        if( h > w ) {
        	bairitu = w / 102;
        	h = bairitu * 64;
        }
        else {
        	/* todo */
        }

		Log.v("androaurex", "modified w, h  = " + w + ", " + h );

//        displayView.setLayoutParams(new LinearLayout.LayoutParams( w, h ));
        displayView.getLayoutParams().height = h;
        displayView.getLayoutParams().width = w;
        displayView.requestLayout();

        Thread t;
//      bairitu= 2;
        jniif = new JniIF( displayView, bairitu);

        setKeyListener( jniif );
        
        t = new Thread(jniif);
        t.start();
    }

    private void setKeyListener( JniIF jniif2 ) {
    	
        View v = (View) this.findViewById(R.id.enterView);

        v.setOnTouchListener( new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
			    	Log.v("androaurex", "Activity.onTouchEvent / Enter Down ");
			    	jniif.pushButtonDown( JniIF.BUTTON_ENTER );
			    }
				else if (event.getAction() == MotionEvent.ACTION_UP) {
			    	Log.v("androaurex", "Activity.onTouchEvent/ Enter UP ");
			    	jniif.pushButtonUp( JniIF.BUTTON_ENTER );
			    }
			    return true;
			}
		});
        v = (View) this.findViewById(R.id.menuView);

        v.setOnTouchListener( new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
			    	Log.v("androaurex", "Activity.onTouchEvent / menu Down ");
			    	jniif.pushButtonDown( JniIF.BUTTON_MENU );
			    }
				else if (event.getAction() == MotionEvent.ACTION_UP) {
			    	Log.v("androaurex", "Activity.onTouchEvent/ Menu UP ");
			    	jniif.pushButtonUp( JniIF.BUTTON_MENU );
			    }
			    return true;
			}
		});

        v = (View) this.findViewById(R.id.filerView);

        v.setOnTouchListener( new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
			    	Log.v("androaurex", "Activity.onTouchEvent / filer Down ");
			    	jniif.pushButtonDown( JniIF.BUTTON_FILER );
			    }
				else if (event.getAction() == MotionEvent.ACTION_UP) {
			    	Log.v("androaurex", "Activity.onTouchEvent/ Menu FILER ");
			    	jniif.pushButtonUp( JniIF.BUTTON_FILER );
			    }
			    return true;
			}
		});
        v = (View) this.findViewById(R.id.elView);

        v.setOnTouchListener( new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
			    	Log.v("androaurex", "Activity.onTouchEvent / el Down ");
			    	jniif.pushButtonDown( JniIF.BUTTON_FILER );
			    }
				else if (event.getAction() == MotionEvent.ACTION_UP) {
			    	Log.v("androaurex", "Activity.onTouchEvent/ el FILER ");
			    	jniif.pushButtonUp( JniIF.BUTTON_FILER );
			    }
			    return true;
			}
		});

        v = (View) this.findViewById(R.id.controlView);        
        v.setOnTouchListener( new KeyControl(jniif2, v ) );
    }
    
   	@Override
   	public void onResume(){
   		super.onResume();

//   		setContentView( sv );
   		   		
   		jniif.aurexjniresume();
    }

   	@Override
   	public void onPause(){
   		super.onPause();
   		   		
   		jniif.aurexjnipause();
    }
}

