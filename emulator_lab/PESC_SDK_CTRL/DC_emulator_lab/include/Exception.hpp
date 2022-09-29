#pragma once
#include <cstdint>

typedef void( *InterruptHandler )( void );

class Exception
{
public:
	void ScuGicEnableIntr( uint32_t id );
	void ScuGicTargetCpu( uint32_t id, uint32_t cpu_no );
	void ScuGicPriority( uint32_t id, uint32_t priority );
	void ScuGicTriggerType( uint32_t id, bool bRisingEdge );
	static void EnableExceptions();
	static void RegisterHandler( uint32_t id, InterruptHandler pHandler );
	static Exception& Instance() { return s_instance; }
private:
	Exception();
	void InitInterruptTable();
	void ScuGicInit();
	static void DefaultHandler();
	static void IrqHandler( void* data );
	static void FiqHandler( void* data );
	static void DataAbortHandler( void* data );
	static void PrefetchAbortHandler( void* data );
	static void SwiIntHandler( void* data );
	static void UnknownHandler( void* data );
	static Exception s_instance;
	InterruptHandler m_interruptTable[96];
};
