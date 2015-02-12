/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>
#include <cassert>

#include <core/thread.hpp>

namespace BlendInt {

	Thread::Thread(const RefPtr<AbstractRunnable>& runnable, bool is_detached)
	: runnable_(runnable),
	  thread_id_(0),
	  detached_(is_detached),
	  result_(0)
	{
		if(runnable_.get() == 0) {
			std::cout << "Thread::Thread(std::auto_ptr<AbstractRunnnable> runnable, bool is_detached) failed at "
					<< " " << __FILE__ << ":" << __LINE__ << "- " << "runnable is NULL" << std::endl;
			exit(-1);
		}
	}

	Thread::Thread(bool is_detached)
	: runnable_(0),
	  thread_id_(0),
	  detached_(is_detached),
	  result_(0)
	{
	}

	Thread::~Thread()
	{
	}

	void Thread::Start()
	{
		int status = pthread_attr_init(&attribute_); // initialize attribute object
		if (status != 0) {
			PrintError("pthread_attr_init failed at", status, __FILE__,
					__LINE__);
			exit(status);
		}

		status = pthread_attr_setscope(&attribute_, PTHREAD_SCOPE_SYSTEM);
		if (status != 0) {
			PrintError("pthread_attr_setscope failed at",
					status, __FILE__, __LINE__);
			exit(status);
		}

		if (detached_) {
			// set the detachstate attribute to detached
			status = pthread_attr_setdetachstate(&attribute_,
					PTHREAD_CREATE_DETACHED);
			if (status != 0){
				PrintError("pthread_attr_setdetachstate failed at",
						status,__FILE__,__LINE__);
				exit(status);
			}

			if (runnable_.get() == NULL) {
				status = pthread_create(&thread_id_,&attribute_,
						Thread::StartThread, (void*) this);
				if (status != 0) {
					PrintError("pthread_create failed at",
							status, __FILE__, __LINE__);
					exit(status);}
			} else {
				status = pthread_create(&thread_id_,&attribute_,
						Thread::StartThreadRunnable, (void*) this);
				if (status != 0) {PrintError("pthread_create failed at",
						status, __FILE__, __LINE__);
				exit(status);}
			}
		} else {
			if (runnable_.get() == 0) {
				int status = pthread_create(&thread_id_,&attribute_,
						Thread::StartThread, (void*)this);
				if (status != 0) {
					PrintError("pthread_create failed at",
							status, __FILE__, __LINE__);
					exit(status);}
			}
			else {
				int status = pthread_create(&thread_id_,&attribute_,
						Thread::StartThreadRunnable, (void*)this);
				if (status != 0) {
					PrintError("pthread_create failed at",
							status, __FILE__, __LINE__);
					exit(status);}
			}
		}

		status = pthread_attr_destroy(&attribute_);
		if (status != 0) {
			PrintError("pthread_attr_destroy failed at",
					status, __FILE__, __LINE__);
			exit(status);
		}
	}

	void* Thread::Join()
	{
		int status = pthread_join(thread_id_,NULL);
		// result was already saved by thread start functions
		if (status != 0) {
			PrintError("pthread_join failed at",
					status, __FILE__, __LINE__);
			//exit(status);
		}
		return result_;
	}

	void Thread::CreateGLContext ()
	{
	}

	void Thread::MakeGLContextCurrent ()
	{
	}

	void Thread::DestroyGLContext ()
	{
	}

	void Thread::SetCompleted()
	{
		/* completion was handled by pthread_join() */
	}

	void Thread::PrintError(const char* msg, int status, const char* filename, int line_no)
	{
		std::cout << msg << ' ' << filename << ":" << line_no
				<< "- " << strerror(status) << std::endl;
	}

	void* Thread::StartThreadRunnable(void* p)
	{
		// thread start function when a Runnable is involved
		Thread* runnable_thread = static_cast<Thread*> (p);
		assert(runnable_thread);
		runnable_thread->result_ = runnable_thread->runnable_->Run();
		runnable_thread->SetCompleted();
		return runnable_thread->result_;
	}

	void* Thread::StartThread(void* p)
	{
		// thread start function when no Runnable is involved
		Thread* thread = static_cast<Thread*> (p);
		assert(thread);
		thread->result_ = thread->Run();
		thread->SetCompleted();
		return thread->result_;
	}

}
