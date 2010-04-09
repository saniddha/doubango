/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tcomp_udvm.h
 * @brief  The machine architecture described in this document.  The UDVM is used to decompress SigComp messages.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TCOMP_UDVM_H
#define TCOMP_UDVM_H

#include "tinysigcomp_config.h"

#include "tcomp_message.h"
#include "tcomp_result.h"
#include "tcomp_headers_index.h"
#include "tcomp_nack_codes.h"
#include "tcomp_statehandler.h"
#include "tcomp_headers_index.h"
#include "tcomp_instructions.h"

#include "tsk_object.h"

#include "tsk_binaryutils.h"

TCOMP_BEGIN_DECLS

#define TCOMP_UDVM_CREATE(_sigCompMessage, stateHandler, lpResult)	tsk_object_new(tcomp_udvm_def_t, (tcomp_message_t*)_sigCompMessage, (tcomp_statehandler_t*)stateHandler, (tcomp_result_t*)lpResult)

#define TCOMP_UDVM_SET_2BYTES_VAL(position, value)	TSK_BINARY_SET_2BYTES(tcomp_buffer_getBufferAtPos(udvm->memory, position), value)
#define TCOMP_UDVM_GET_2BYTES_VAL(position)			TSK_BINARY_GET_2BYTES(tcomp_buffer_getBufferAtPos(udvm->memory, position))

#if defined(DEBUG) || defined(_DEBUG)
#	define TCOMP_UDVM_DEBUG_PRINT(size)		tcomp_buffer_nprint(udvm->memory, size)
#else
#	define TCOMP_UDVM_DEBUG_PRINT(size)		((void)size)
#endif

#define TCOMP_UDVM_GET_SIZE()				tcomp_buffer_getSize(udvm->memory)
#define TCOMP_UDVM_GET_BUFFER()				tcomp_buffer_getBuffer(udvm->memory)
#define TCOMP_UDVM_GET_BUFFER_AT(position)	tcomp_buffer_getBufferAtPos(udvm->memory, position)

typedef struct tcomp_udvm_s
{
	TSK_DECLARE_OBJECT;

	unsigned isOK:1;
	tcomp_message_t *sigCompMessage;
	tcomp_statehandler_t *stateHandler;
	tcomp_result_t *lpResult;

	uint32_t maximum_UDVM_cycles; // RFC3320-Section_8.6
	uint64_t consumed_cycles;
	
	tcomp_buffer_handle_t *memory;
	//header_udvm_memory memoryHeader; // RFC3320-Section_7.2 - // Default constructor will initial values (sip default)
	
	uint16_t executionPointer;
	uint16_t last_memory_address_of_instruction;
}
tcomp_udvm_t;


tsk_bool_t tcomp_udvm_decompress(tcomp_udvm_t *udvm);

/*
*	Operands
*/
uint16_t tcomp_udvm_opget_literal_param(tcomp_udvm_t *udvm);
uint16_t tcomp_udvm_opget_reference_param(tcomp_udvm_t *udvm);
uint16_t tcomp_udvm_opget_multitype_param(tcomp_udvm_t *udvm);
uint16_t tcomp_udvm_opget_address_param(tcomp_udvm_t *udvm, uint16_t memory_address_of_instruction);

/*
* ByteCopy
*/
int tcomp_udvm_bytecopy_self(tcomp_udvm_t *udvm, uint16_t *destination, uint16_t source, uint16_t size_tocopy);
int tcomp_udvm_bytecopy_to(tcomp_udvm_t *udvm, uint16_t destination, const uint8_t* source, uint16_t size_tocopy);
int tcomp_udvm_bytecopy_from(tcomp_udvm_t *udvm, uint8_t* destination, uint16_t source, uint16_t size_tocopy);

/*
* State Management
*/
int tcomp_udvm_byteCopy_TempStates(tcomp_udvm_t *udvm);
int tcomp_udvm_createTempState(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, 
	uint16_t minimum_access_length, uint16_t state_retention_priority, int end_msg);

/*
* Nack creation
*/
void tcomp_udvm_createNackInfo(tcomp_udvm_t *udvm, uint8_t reasonCode, tcomp_buffer_handle_t* lpDetails, int16_t memory_address_of_instruction);
#define tcomp_udvm_createNackInfo2(udvm, reasonCode) tcomp_udvm_createNackInfo(udvm, reasonCode, 0, -1)
#define tcomp_udvm_createNackInfo3(udvm, reasonCode, lpDetails) tcomp_udvm_createNackInfo(udvm, reasonCode, lpDetails, -1)

/*
* Instructions
*/
int TCOMP_UDVM_EXEC_INST__DECOMPRESSION_FAILURE(tcomp_udvm_t *udvm);
int TCOMP_UDVM_EXEC_INST__AND(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__OR(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__NOT(tcomp_udvm_t *udvm, uint16_t operand_1);
int TCOMP_UDVM_EXEC_INST__LSHIFT(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__RSHIFT(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__ADD(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__SUBTRACT(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__MULTIPLY(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__DIVIDE(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__REMAINDER(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2);
int TCOMP_UDVM_EXEC_INST__SORT_ASCENDING(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k);
int TCOMP_UDVM_EXEC_INST__SORT_DESCENDING(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k);
int TCOMP_UDVM_EXEC_INST__SHA_1(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination);
int TCOMP_UDVM_EXEC_INST__LOAD(tcomp_udvm_t *udvm, uint16_t address, uint16_t value);
int TCOMP_UDVM_EXEC_INST__MULTILOAD(tcomp_udvm_t *udvm, uint16_t address, uint16_t n);
int TCOMP_UDVM_EXEC_INST__PUSH(tcomp_udvm_t *udvm, int16_t value);
#define TCOMP_UDVM_EXEC_INST__PUSH2(udvm) TCOMP_UDVM_EXEC_INST__PUSH(udvm, -1)
int TCOMP_UDVM_EXEC_INST__POP(tcomp_udvm_t *udvm, uint16_t* value);
#define TCOMP_UDVM_EXEC_INST__POP2(udvm) TCOMP_UDVM_EXEC_INST__POP(udvm, 0)
int TCOMP_UDVM_EXEC_INST__COPY(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination);
int TCOMP_UDVM_EXEC_INST__COPY_LITERAL(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination);
int TCOMP_UDVM_EXEC_INST__COPY_OFFSET(tcomp_udvm_t *udvm, uint16_t offset, uint16_t length, uint16_t destination);
int TCOMP_UDVM_EXEC_INST__MEMSET(tcomp_udvm_t *udvm, uint16_t address, uint16_t length, uint16_t start_value, uint16_t offset);
int TCOMP_UDVM_EXEC_INST__JUMP(tcomp_udvm_t *udvm, int16_t address);
#define TCOMP_UDVM_EXEC_INST__JUMP2(udvm) TCOMP_UDVM_EXEC_INST__JUMP(udvm, -1)
int TCOMP_UDVM_EXEC_INST__COMPARE(tcomp_udvm_t *udvm, uint16_t value_1, uint16_t value_2, uint16_t address_1, uint16_t address_2, uint16_t address_3);
int TCOMP_UDVM_EXEC_INST__CALL(tcomp_udvm_t *udvm, uint16_t address);
int TCOMP_UDVM_EXEC_INST__RETURN(tcomp_udvm_t *udvm);
int TCOMP_UDVM_EXEC_INST__SWITCH(tcomp_udvm_t *udvm, uint16_t n, uint16_t j);
int TCOMP_UDVM_EXEC_INST__CRC(tcomp_udvm_t *udvm, uint16_t value, uint16_t position, uint16_t length, uint16_t address);
int TCOMP_UDVM_EXEC_INST__INPUT_BYTES(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address);
int TCOMP_UDVM_EXEC_INST__INPUT_BITS(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address);
int TCOMP_UDVM_EXEC_INST__INPUT_HUFFMAN(tcomp_udvm_t *udvm, uint16_t destination, uint16_t address, uint16_t n);
int TCOMP_UDVM_EXEC_INST__STATE_ACCESS(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length, uint16_t state_begin, uint16_t state_length, uint16_t state_address, uint16_t state_instruction);
int TCOMP_UDVM_EXEC_INST__STATE_CREATE(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority);
int TCOMP_UDVM_EXEC_INST__STATE_FREE(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length);
int TCOMP_UDVM_EXEC_INST__OUTPUT(tcomp_udvm_t *udvm, uint16_t output_start, uint16_t output_length);
int TCOMP_UDVM_EXEC_INST__END_MESSAGE(tcomp_udvm_t *udvm, uint16_t requested_feedback_location, uint16_t returned_parameters_location, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority);


TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_udvm_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_UDVM_H */
