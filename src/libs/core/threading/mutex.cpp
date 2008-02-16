
/***************************************************************************
 *  mutex.cpp - implementation of mutex, based on pthreads
 *
 *  Generated: Thu Sep 14 17:03:57 2006
 *  Copyright  2006  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
 */

#include <core/threading/mutex.h>
#include <core/threading/mutex_data.h>

#include <pthread.h>


/** @class Mutex core/threading/mutex.h
 * Mutex mutual exclusion lock.
 * This class is used in a multi-threading environment to lock access to
 * resources. This is needed to prevent two threads from modifying a value
 * at the same time or to prevent a thread from getting a dirty copy of
 * a piece of data (the reader reads while a writer is writing, this could
 * leave the data in a state where the reader reads half of the new and half
 * of the old data).
 *
 * As a rule of thumb you should lock the mutex as short as possible and as
 * long as needed. Locking the mutex too long will lead in a bad performance
 * of the multi-threaded application because many threads are waiting for
 * the lock and are not doing anything useful.
 * If you do not lock enough code (and so serialize it) it will cause pain
 * and errors.
 *
 * @ingroup Threading
 * @ingroup FCL
 * @see example_mutex_count.cpp
 *
 * @author Tim Niemueller
 */


/** Constructor */
Mutex::Mutex()
{
  mutex_data = new MutexData();
  pthread_mutex_init(&(mutex_data->mutex), NULL);
}

/** Destructor */
Mutex::~Mutex()
{
  pthread_mutex_destroy(&(mutex_data->mutex));
  delete mutex_data;
  mutex_data = NULL;
}


/** Lock this mutex.
 * A call to lock() will block until the lock on the mutex could be aquired.
 * If you want to avoid see consider using try_lock().
 */
void
Mutex::lock()
{
  pthread_mutex_lock(&(mutex_data->mutex));
#ifdef DEBUG_THREADING
  // do not switch order, lock holder must be protected with this mutex!
  mutex_data->set_lock_holder();
#endif
}


/** Tries to lock the mutex.
 * This can also be used to check if a mutex is locked. The code for this
 * can be:
 *
 * @code
 * bool locked = false;
 * if ( mutex->try_lock() ) {
 *   mutex->unlock();
 *   locked = true;
 * }
 * @endcode
 *
 * This cannot be implemented in Mutex in a locked() method since this
 * would lead to race conditions in many situations.
 *
 * @return true, if the mutex could be locked, false otherwise.
 */
bool
Mutex::try_lock()
{
  if (pthread_mutex_trylock(&(mutex_data->mutex)) == 0) {
#ifdef DEBUG_THREADING
    mutex_data->set_lock_holder();
#endif
    return true;
  } else {
    return false;
  }
}


/** Unlock the mutex. */
void
Mutex::unlock()
{
#ifdef DEBUG_THREADING
  mutex_data->unset_lock_holder();
  // do not switch order, lock holder must be protected with this mutex!
#endif
  pthread_mutex_unlock(&(mutex_data->mutex));
}
