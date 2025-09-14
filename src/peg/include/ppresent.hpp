#ifndef _PPRESENT_
#define _PPRESENT_

/*--------------------------------------------------------------------------*/
// The maximum number of things that will ever simultaneously try to capture
// the mouse pointer
/*--------------------------------------------------------------------------*/

#define MAX_POINTER_CAPTURE_NESTING 8

#ifdef PEG_MULTITHREAD

struct PegTaskInfo {
  PegTaskInfo PEGFAR *pNext;
  PegThing PEGFAR *pThing;

  PEG_TASK_TYPE pTask;
  PEG_QUEUE_TYPE pQueue;
};

#endif

#endif