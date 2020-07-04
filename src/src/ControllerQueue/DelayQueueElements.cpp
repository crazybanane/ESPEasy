#include "DelayQueueElements.h"


#ifdef USES_MQTT
ControllerDelayHandlerStruct<MQTT_queue_element> MQTTDelayHandler;
#endif // USES_MQTT


/*********************************************************************************************\
* C001_queue_element for queueing requests for C001.
\*********************************************************************************************/
#ifdef USES_C001
# define C001_queue_element simple_queue_element_string_only
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(00,  1)
#endif // ifdef USES_C001

/*********************************************************************************************\
* C003_queue_element for queueing requests for C003 Nodo Telnet.
\*********************************************************************************************/
#ifdef USES_C003
# define C003_queue_element simple_queue_element_string_only
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(00,  3)
#endif // ifdef USES_C003

#ifdef USES_C004
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(00,  4)
#endif // ifdef USES_C004

#ifdef USES_C007
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(00,  7)
#endif // ifdef USES_C007



/*********************************************************************************************\
* C008_queue_element for queueing requests for 008: Generic HTTP
* Using queue_element_single_value_base
\*********************************************************************************************/
#ifdef USES_C008
# define C008_queue_element queue_element_single_value_base
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(00,  8)
#endif // ifdef USES_C008

#ifdef USES_C009
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(00,  9)
#endif // ifdef USES_C009


/*********************************************************************************************\
* C010_queue_element for queueing requests for 010: Generic UDP
* Using queue_element_single_value_base
\*********************************************************************************************/
#ifdef USES_C010
# define C010_queue_element queue_element_single_value_base
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP( 0, 10)
#endif // ifdef USES_C010



/*********************************************************************************************\
* C011_queue_element for queueing requests for 011: Generic HTTP Advanced
\*********************************************************************************************/
#ifdef USES_C011
# define C011_queue_element simple_queue_element_string_only
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP( 0, 11)
#endif // ifdef USES_C011


/*********************************************************************************************\
* C012_queue_element for queueing requests for 012: Blynk
* Using queue_element_single_value_base
\*********************************************************************************************/
#ifdef USES_C012
# define C012_queue_element queue_element_single_value_base
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP( 0, 12)
#endif // ifdef USES_C012

/*
 #ifdef USES_C013
   DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 13)
 #endif
 */

/*
 #ifdef USES_C014
   DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 14)
 #endif
 */


#ifdef USES_C015
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 15)
#endif // ifdef USES_C015



#ifdef USES_C016
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 16)
#endif // ifdef USES_C016


#ifdef USES_C017
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 17)
#endif // ifdef USES_C017

#ifdef USES_C018
DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 18)
#endif // ifdef USES_C018


/*
 #ifdef USES_C019
   DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 19)
 #endif
 */

/*
 #ifdef USES_C020
   DEFINE_Cxxx_DELAY_QUEUE_MACRO_CPP(0, 20)
 #endif
 */


// When extending this, also extend in Scheduler.cpp:
// void process_interval_timer(unsigned long id, unsigned long lasttimer)