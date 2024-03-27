/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/bluetooth/mesh.h>
#include "chat_cli.h"
#include "mesh/net.h"
#include <string.h>

#include <zephyr/logging/log.h>
// Using the MAIN FILE log module (can only be one log module)
LOG_MODULE_DECLARE(chat);

/* 
BUILD_ASSERT is a macro to determine if BT_MESH_CHAT_CLI_OP_MESSAGE is less than
or equal to the maximum size of a recieved Service Data Unit (SDU) in our case
it will be BT_MESH_RX_SDU_MAX.
If the value becomes false then give the following message:
"The message must fit inside an application SDU."

This is for RX
*/
BUILD_ASSERT(BT_MESH_MODEL_BUF_LEN(BT_MESH_CHAT_CLI_OP_MESSAGE,
				   BT_MESH_CHAT_CLI_MSG_MAXLEN_MESSAGE) <=
		    BT_MESH_RX_SDU_MAX,
	     "The message must fit inside an application SDU.");

/* 
BUILD_ASSERT is a macro to determine if BT_MESH_CHAT_CLI_OP_MESSAGE is less than
or equal to the maximum size of a recieved Service Data Unit (SDU) in our case
it will be BT_MESH_RX_SDU_MAX
If the value becomes false then give the following message:
"The message must fit inside an application SDU."

This is for TX
*/
BUILD_ASSERT(BT_MESH_MODEL_BUF_LEN(BT_MESH_CHAT_CLI_OP_MESSAGE,
				   BT_MESH_CHAT_CLI_MSG_MAXLEN_MESSAGE) <=
		    BT_MESH_TX_SDU_MAX,
	     "The message must fit inside an application SDU.");

/*
The entire purpose of the function is to clear the buffer and get the new message.
Where the message will be encoded and add 8 bit at the end of the buffer (helps for
traffic/debugging purposes)

"opcode" (operation code) refers to a unique identifier that represents a specific
operation or action within a Bluetooth Mesh model

Reference: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/v1.6-branch/zephyr/reference/networking/net_buf.html
*/
static void encode_presence(struct net_buf_simple *buf,
			    enum bt_mesh_chat_cli_presence presence)
{
	// Initialize a model message, clear the buffer then encode the given opcode (BT_MESH_CHAT_CLI_OP_PRESENCE)
	bt_mesh_model_msg_init(buf, BT_MESH_CHAT_CLI_OP_PRESENCE);

	// Add 8-bit at the end of the buffer
	net_buf_simple_add_u8(buf, presence);
}


/* 

*/
static const uint8_t *extract_msg(struct net_buf_simple *buf)
{
	// Get the last element value of data and make it null since the extracted message is not in proper format[buf->data mean buf is a pointer struct] 
	buf->data[buf->len - 1] = '\0';
	
	// Since the information is in the proper format then return the value
	return net_buf_simple_pull_mem(buf, buf->len);
}

/*
This sequence of actions involves receiving a message input from the user, ensuring it conforms to the required format, 
and then processing the message to transition to the next state of operation.
*/
static int handle_message(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf)
{
	// Create a pointer "chat" that points to the user data associated with the "model" 
	struct bt_mesh_chat_cli *chat = model->user_data;
	
	// Create a pointer "msg"
	const uint8_t *msg;

	// Called the function "extract_msg" for the proper format "msg"
	msg = extract_msg(buf);

	// From chat (pointer struct) get the data handlers (pointer struct) where we get the data from message
	if (chat->handlers->message) {
		/*
		Just to process the relevant information/arguements

		Chat: Allowing the message handling function to have access to the "chat" structure
		ctx: Pointer to a struct (struct bt_mesh_msg_ctx) containing the context information 
		about the message being handle (such as source and destination)
		msg: This is a pointer to the message data being handled (unsure couldnt find the
		struct being declared)
		*/
		chat->handlers->message(chat, ctx, msg);
	}

	return 0;
}

/* .. include_startingpoint_chat_cli_rst_1 */
static void send_message_reply(struct bt_mesh_chat_cli *chat,
			     struct bt_mesh_msg_ctx *ctx)
{
	// Interpretering the message opcode (what type of message and where is should go)
	BT_MESH_MODEL_BUF_DEFINE(msg, BT_MESH_CHAT_CLI_OP_MESSAGE_REPLY,
				 BT_MESH_CHAT_CLI_MSG_LEN_MESSAGE_REPLY);

	// The code prepares the buffer message for sending a message to reply
	bt_mesh_model_msg_init(&msg, BT_MESH_CHAT_CLI_OP_MESSAGE_REPLY);

	// Making the function into a NULL/EMPTY state
	(void)bt_mesh_model_send(chat->model, ctx, &msg, NULL, NULL);
}

/*
This sequence of actions involves receiving a message input from the user, ensuring it conforms to the required format, 
and then processing the message to transition to the next state of operation.
*/
static int handle_private_message(struct bt_mesh_model *model,
				  struct bt_mesh_msg_ctx *ctx,
				  struct net_buf_simple *buf)
{
	// Create a pointer "chat" that points to the user data associated with the "model" 
	struct bt_mesh_chat_cli *chat = model->user_data;

	// Create a pointer "msg"
	const uint8_t *msg;

	// Called the function "extract_msg" for the proper format "msg"
	msg = extract_msg(buf);

	// From chat (pointer struct) get the data handlers (pointer struct) where we get the data from message
	if (chat->handlers->private_message) {
		/*
		Just to process the relevant information/arguements

		Chat: Allowing the message handling function to have access to the "chat" structure
		ctx: Pointer to a struct (struct bt_mesh_msg_ctx) containing the context information 
		about the message being handle (such as source and destination)
		msg: This is a pointer to the message data being handled (unsure couldnt find the
		struct being declared)
		*/
		chat->handlers->private_message(chat, ctx, msg);
	}

	// Calls the function to send the message
	send_message_reply(chat, ctx);

	return 0;
}

/*
Function to handle a message reply within the Bluetooth Mesh chat application

include_endpoint_chat_cli_rst_1
*/
static int handle_message_reply(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
				struct net_buf_simple *buf)
{
	// Extract user data associated with the mesh model, presumed to be of type bt_mesh_chat_cli
	struct bt_mesh_chat_cli *chat = model->user_data;

	// Check if there's a callback function registered for handling message replies
	if (chat->handlers->message_reply) {
		// Invoke the message reply callback function, passing the chat structure and message context
		chat->handlers->message_reply(chat, ctx);
	}

	return 0;
}

/*
Function to handle presence(status or ) messages within the Bluetooth Mesh chat application
*/
static int handle_presence(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
			   struct net_buf_simple *buf)
{
	// Extract user data associated with the mesh model, presumed to be of type bt_mesh_chat_cli
	struct bt_mesh_chat_cli *chat = model->user_data;

	// Declare a variable to store the received presence value
	enum bt_mesh_chat_cli_presence presence;

	// Extract the presence value from the network buffer
	presence = net_buf_simple_pull_u8(buf);

	// Check if there's a callback function registered for handling presence messages
	if (chat->handlers->presence) {
		// Invoke the presence callback function, passing the chat structure, message context, and presence value
		chat->handlers->presence(chat, ctx, presence);
	}

	return 0;
}

/*
Function to handle presence get requests within the Bluetooth Mesh chat application
*/
static int handle_presence_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf)
{
	// Extract user data associated with the mesh model, presumed to be of type bt_mesh_chat_cli
	struct bt_mesh_chat_cli *chat = model->user_data;

	// Define a network buffer to store the outgoing presence message
	BT_MESH_MODEL_BUF_DEFINE(msg, BT_MESH_CHAT_CLI_OP_PRESENCE,
				 BT_MESH_CHAT_CLI_MSG_LEN_PRESENCE);

	// Encode the current presence value into the network buffer
	encode_presence(&msg, chat->presence);

	// Send the encoded presence message to the requester
	(void) bt_mesh_model_send(chat->model, ctx, &msg, NULL, NULL);

	return 0;
}

/* 
Define an array of bt_mesh_model_op structures representing the operations
supported by a Bluetooth Mesh chat client model.

include_startingpoint_chat_cli_rst_2 
*/
const struct bt_mesh_model_op _bt_mesh_chat_cli_op[] = {
	// Operation for sending a regular message
	{
		BT_MESH_CHAT_CLI_OP_MESSAGE,                                // Opcode
		BT_MESH_LEN_MIN(BT_MESH_CHAT_CLI_MSG_MINLEN_MESSAGE),       // Minimum length
		handle_message                                              // Handler function
	},

	// Operation for sending a private message
	{
		BT_MESH_CHAT_CLI_OP_PRIVATE_MESSAGE,                        // Opcode
		BT_MESH_LEN_MIN(BT_MESH_CHAT_CLI_MSG_MINLEN_MESSAGE),       // Minimum length
		handle_private_message                                      // Handler function
	},

	// Operation for replying to a message
	{
		BT_MESH_CHAT_CLI_OP_MESSAGE_REPLY,                          // Opcode
		BT_MESH_LEN_EXACT(BT_MESH_CHAT_CLI_MSG_LEN_MESSAGE_REPLY),  // Exact length
		handle_message_reply                                        // Handler function 
	},

	// Operation for indicating presence status
	{
		BT_MESH_CHAT_CLI_OP_PRESENCE,                               // Opcode
		BT_MESH_LEN_EXACT(BT_MESH_CHAT_CLI_MSG_LEN_PRESENCE),       // Exact length 
		handle_presence                                             // Handler function 
	},

	// Operation for requesting presence status
	{
		BT_MESH_CHAT_CLI_OP_PRESENCE_GET,                           // Opcode 
		BT_MESH_LEN_EXACT(BT_MESH_CHAT_CLI_MSG_LEN_PRESENCE_GET),   // Exact length 
		handle_presence_get                                         // Handler function  
	},
	
	// End marker
	BT_MESH_MODEL_OP_END,
};


/*
Handler function for updating the presence status of a Bluetooth Mesh chat client model

include_endpoint_chat_cli_rst_2 
*/
static int bt_mesh_chat_cli_update_handler(struct bt_mesh_model *model)
{
	// Retrieve the chat client data associated with the model
	struct bt_mesh_chat_cli *chat = model->user_data;

	// Update the message buffer with the current presence status
	encode_presence(model->pub->msg, chat->presence);

	return 0;
}

/* .. include_startingpoint_chat_cli_rst_3 */
#ifdef CONFIG_BT_SETTINGS
static int bt_mesh_chat_cli_settings_set(struct bt_mesh_model *model,
					 const char *name,
					 size_t len_rd,
					 settings_read_cb read_cb,
					 void *cb_arg)
{
	struct bt_mesh_chat_cli *chat = model->user_data;

	if (name) {
		return -ENOENT;
	}

	ssize_t bytes = read_cb(cb_arg, &chat->presence,
				sizeof(chat->presence));
	if (bytes < 0) {
		return bytes;
	}

	if (bytes != 0 && bytes != sizeof(chat->presence)) {
		return -EINVAL;
	}

	return 0;
}
#endif
/* .. include_endpoint_chat_cli_rst_3 */

/* .. include_startingpoint_chat_cli_rst_4 */
static int bt_mesh_chat_cli_init(struct bt_mesh_model *model)
{
	struct bt_mesh_chat_cli *chat = model->user_data;

	chat->model = model;

	net_buf_simple_init_with_data(&chat->pub_msg, chat->buf,
				      sizeof(chat->buf));
	chat->pub.msg = &chat->pub_msg;
	chat->pub.update = bt_mesh_chat_cli_update_handler;

	return 0;
}
/* .. include_endpoint_chat_cli_rst_4 */

/* .. include_startingpoint_chat_cli_rst_5 */
static int bt_mesh_chat_cli_start(struct bt_mesh_model *model)
{
	struct bt_mesh_chat_cli *chat = model->user_data;

	if (chat->handlers->start) {
		chat->handlers->start(chat);
	}

	return 0;
}
/* .. include_endpoint_chat_cli_rst_5 */

/* .. include_startingpoint_chat_cli_rst_6 */
static void bt_mesh_chat_cli_reset(struct bt_mesh_model *model)
{
	struct bt_mesh_chat_cli *chat = model->user_data;

	chat->presence = BT_MESH_CHAT_CLI_PRESENCE_AVAILABLE;

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		(void) bt_mesh_model_data_store(model, true, NULL, NULL, 0);
	}
}
/* .. include_endpoint_chat_cli_rst_6 */

/* .. include_startingpoint_chat_cli_rst_7 */
const struct bt_mesh_model_cb _bt_mesh_chat_cli_cb = {
	.init = bt_mesh_chat_cli_init,
	.start = bt_mesh_chat_cli_start,
#ifdef CONFIG_BT_SETTINGS
	.settings_set = bt_mesh_chat_cli_settings_set,
#endif
	.reset = bt_mesh_chat_cli_reset,
};
/* .. include_endpoint_chat_cli_rst_7 */

/* .. include_startingpoint_chat_cli_rst_8 */
int bt_mesh_chat_cli_presence_set(struct bt_mesh_chat_cli *chat,
			 enum bt_mesh_chat_cli_presence presence)
{
	if (presence != chat->presence) {
		chat->presence = presence;

		if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
			(void) bt_mesh_model_data_store(chat->model, true,
						NULL, &presence,
						sizeof(chat->presence));
		}
	}

	encode_presence(chat->model->pub->msg, chat->presence);

	return bt_mesh_model_publish(chat->model);
}
/* .. include_endpoint_chat_cli_rst_8 */

int bt_mesh_chat_cli_presence_get(struct bt_mesh_chat_cli *chat,
				  uint16_t addr)
{
	struct bt_mesh_msg_ctx ctx = {
		.addr = addr,
		.app_idx = chat->model->keys[0],
		.send_ttl = BT_MESH_TTL_DEFAULT,
		.send_rel = true,
	};

	BT_MESH_MODEL_BUF_DEFINE(buf, BT_MESH_CHAT_CLI_OP_PRESENCE_GET,
				 BT_MESH_CHAT_CLI_MSG_LEN_PRESENCE_GET);
	bt_mesh_model_msg_init(&buf, BT_MESH_CHAT_CLI_OP_PRESENCE_GET);

	return bt_mesh_model_send(chat->model, &ctx, &buf, NULL, NULL);
}

int bt_mesh_chat_cli_message_send(struct bt_mesh_chat_cli *chat,
				  const uint8_t *msg)
{
	struct net_buf_simple *buf = chat->model->pub->msg;

	bt_mesh_model_msg_init(buf, BT_MESH_CHAT_CLI_OP_MESSAGE);

	net_buf_simple_add_mem(buf, msg,
			       strnlen(msg,
				       CONFIG_BT_MESH_CHAT_CLI_MESSAGE_LENGTH));
	net_buf_simple_add_u8(buf, '\0');

	return bt_mesh_model_publish(chat->model);
}

/* .. include_startingpoint_chat_cli_rst_9 */
int bt_mesh_chat_cli_private_message_send(struct bt_mesh_chat_cli *chat,
					  uint16_t addr,
					  const uint8_t *msg)
{
	struct bt_mesh_msg_ctx ctx = {
		.addr = addr,
		.app_idx = chat->model->keys[0],
		.send_ttl = BT_MESH_TTL_DEFAULT,
		.send_rel = true,
	};

	BT_MESH_MODEL_BUF_DEFINE(buf, BT_MESH_CHAT_CLI_OP_PRIVATE_MESSAGE,
				 BT_MESH_CHAT_CLI_MSG_MAXLEN_MESSAGE);
	bt_mesh_model_msg_init(&buf, BT_MESH_CHAT_CLI_OP_PRIVATE_MESSAGE);

	net_buf_simple_add_mem(&buf, msg,
			       strnlen(msg,
				       CONFIG_BT_MESH_CHAT_CLI_MESSAGE_LENGTH));
	net_buf_simple_add_u8(&buf, '\0');

	return bt_mesh_model_send(chat->model, &ctx, &buf, NULL, NULL);
}
/* .. include_endpoint_chat_cli_rst_9 */
