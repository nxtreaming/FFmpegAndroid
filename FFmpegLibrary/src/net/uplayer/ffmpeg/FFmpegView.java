/*
 * FFmpegView.java
 * Copyright (c) 2012 Jacek Marchwicki
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

package net.uplayer.ffmpeg;

import net.uplayer.ffmpeg.FFmpegPlayer.RenderedFrame;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

public class FFmpegView extends View implements FFmpegDisplay  {
	private FFmpegPlayer mMpegPlayer = null;
	private Object mMpegPlayerLock = new Object();
	private TutorialThread mThread = null;
	private Paint mPaint;

	RenderedFrame renderFrame;

	public FFmpegView(Context context) {
		this(context, null, 0);
	}

	public FFmpegView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public FFmpegView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		mPaint = new Paint();
		mPaint.setTextSize(32);
		mPaint.setColor(Color.RED);
	}

	public void init(){
		if (mMpegPlayer != null)
			mMpegPlayer= null;
	}

	@Override
	public void setMpegPlayer(FFmpegPlayer fFmpegPlayer) {
		if (mMpegPlayer != null)
			throw new RuntimeException("setMpegPlayer could not be called twice");
		
		synchronized (mMpegPlayerLock) {
			this.mMpegPlayer = fFmpegPlayer;
			mMpegPlayerLock.notifyAll();
			this.mMpegPlayer.renderFrameStart();
			mThread = new TutorialThread();
			mThread.setRunning(true);
			mThread.setStop(false);
			mThread.start();
		}
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		if(renderFrame == null)
			return;
		try {
			canvas.drawColor(Color.BLACK);
			canvas.save();
			int width = this.getWidth();
			int height = this.getHeight();
			float ratiow = width / (float) renderFrame.width;
			float ratioh = height / (float) renderFrame.height;
			float ratio = ratiow > ratioh ? ratioh : ratiow;
			float moveX = ((renderFrame.width * ratio - width) / 2.0f);
			float moveY = ((renderFrame.height * ratio - height) / 2.0f);
			canvas.translate(-moveX, -moveY);
			canvas.scale(ratio, ratio);

			canvas.drawBitmap(renderFrame.bitmap, 0, 0, mPaint);
			canvas.restore();
		} catch(Exception e) {
			e.printStackTrace();
		}finally {
			mMpegPlayer.releaseFrame();
		}
	}

	@Override
	protected void onAttachedToWindow() {
		super.onAttachedToWindow();
//		this.mpegPlayer.renderFrameStart();
	}

	@Override
	protected void onDetachedFromWindow() {
		super.onDetachedFromWindow();
//		if (mThread != null) {
//			this.mMpegPlayer.renderFrameStop();
//			mThread.setRunning(false);
//			mThread.interrupt();
//		}
	}

	class TutorialThread extends Thread {
		private volatile boolean mRun = false;
		private volatile boolean mStop = false;

		public TutorialThread() {
		}

		public synchronized void setRunning(boolean run) {
			mRun = run;
		}

		public synchronized void setStop(boolean stop) {
			mStop = stop;
		}

		public synchronized boolean isRunning() {
			return (mRun && !mStop);
		}

		@Override
		public void run() {
			while (isRunning()) {
				try {
					synchronized (mMpegPlayerLock) {
						while (mMpegPlayerLock == null)
							mMpegPlayerLock.wait();
						if (isRunning()) {
							renderFrame(mMpegPlayer);
							Log.d("renderFrame", "renderFrame+++++++");
						}
					}
				} catch (InterruptedException e) {
				}
			}
		}

		private void renderFrame(FFmpegPlayer mpegPlayer) throws InterruptedException {
			 renderFrame = mpegPlayer.renderFrame();

			//if render is interrupted by user, it will return |null|, we just ignore it
			if (renderFrame == null) {
				setStop(true);
				return; //throw new RuntimeException();
			}
			if (renderFrame.bitmap == null) {
				setStop(true);
				return;//throw new RuntimeException();
			}
			try {
			     postInvalidate();
			} finally {
			}
		}
	}

	@Override
	public void destroy() {
		// TODO Auto-generated method stub
		if (mThread != null) {
			this.mMpegPlayer.renderFrameStop();
			mThread.setRunning(false);
			mThread.interrupt();
			renderFrame = null;
		}
	}
}
