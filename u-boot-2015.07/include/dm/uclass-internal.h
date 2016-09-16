/*
 * Copyright (c) 2013 Google, Inc
 *
 * (C) Copyright 2012
 * Pavel Herrmann <morpheus.ibis@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _DM_UCLASS_INTERNAL_H
#define _DM_UCLASS_INTERNAL_H

/**
 * uclass_get_device_tail() - handle the end of a get_device call
 *
 * This handles returning an error or probing a device as needed.
 *
 * @dev: Device that needs to be probed
 * @ret: Error to return. If non-zero then the device is not probed
 * @devp: Returns the value of 'dev' if there is no error
 * @return ret, if non-zero, else the result of the device_probe() call
 */
int uclass_get_device_tail(struct udevice *dev, int ret, struct udevice **devp);

/**
 * uclass_find_device() - Return n-th child of uclass
 * @id:		Id number of the uclass
 * @index:	Position of the child in uclass's list
 * #devp:	Returns pointer to device, or NULL on error
 *
 * The device is not prepared for use - this is an internal function.
 * The function uclass_get_device_tail() can be used to probe the device.
 *
 * @return the uclass pointer of a child at the given index or
 * return NULL on error.
 */
int uclass_find_device(enum uclass_id id, int index, struct udevice **devp);

/**
 * uclass_find_first_device() - Return the first device in a uclass
 * @id:		Id number of the uclass
 * #devp:	Returns pointer to device, or NULL on error
 *
 * The device is not prepared for use - this is an internal function.
 * The function uclass_get_device_tail() can be used to probe the device.
 *
 * @return 0 if OK (found or not found), -1 on error
 */
int uclass_find_first_device(enum uclass_id id, struct udevice **devp);

/**
 * uclass_find_next_device() - Return the next device in a uclass
 * @devp: On entry, pointer to device to lookup. On exit, returns pointer
 * to the next device in the same uclass, or NULL if none
 *
 * The device is not prepared for use - this is an internal function.
 * The function uclass_get_device_tail() can be used to probe the device.
 *
 * @return 0 if OK (found or not found), -1 on error
 */
int uclass_find_next_device(struct udevice **devp);

/**
 * uclass_find_device_by_name() - Find uclass device based on ID and name
 *
 * This searches for a device with the exactly given name.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @name: name of a device to find
 * @devp: Returns pointer to device (the first one with the name)
 * @return 0 if OK, -ve on error
 */
int uclass_find_device_by_name(enum uclass_id id, const char *name,
			       struct udevice **devp);

/**
 * uclass_find_device_by_seq() - Find uclass device based on ID and sequence
 *
 * This searches for a device with the given seq or req_seq.
 *
 * For seq, if an active device has this sequence it will be returned.
 * If there is no such device then this will return -ENODEV.
 *
 * For req_seq, if a device (whether activated or not) has this req_seq
 * value, that device will be returned. This is a strong indication that
 * the device will receive that sequence when activated.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @seq_or_req_seq: Sequence number to find (0=first)
 * @find_req_seq: true to find req_seq, false to find seq
 * @devp: Returns pointer to device (there is only one per for each seq)
 * @return 0 if OK, -ve on error
 */
int uclass_find_device_by_seq(enum uclass_id id, int seq_or_req_seq,
			      bool find_req_seq, struct udevice **devp);

/**
 * uclass_bind_device() - Associate device with a uclass
 *
 * Connect the device into uclass's list of devices.
 *
 * @dev:	Pointer to the device
 * #return 0 on success, -ve on error
 */
int uclass_bind_device(struct udevice *dev);

/**
 * uclass_unbind_device() - Deassociate device with a uclass
 *
 * Disconnect the device from uclass's list of devices.
 *
 * @dev:	Pointer to the device
 * #return 0 on success, -ve on error
 */
#ifdef CONFIG_DM_DEVICE_REMOVE
int uclass_unbind_device(struct udevice *dev);
#else
static inline int uclass_unbind_device(struct udevice *dev) { return 0; }
#endif

/**
 * uclass_pre_probe_device() - Deal with a device that is about to be probed
 *
 * Perform any pre-processing that is needed by the uclass before it can be
 * probed. This includes the uclass' pre-probe() method and the parent
 * uclass' child_pre_probe() method.
 *
 * @dev:	Pointer to the device
 * #return 0 on success, -ve on error
 */
int uclass_pre_probe_device(struct udevice *dev);

/**
 * uclass_post_probe_device() - Deal with a device that has just been probed
 *
 * Perform any post-processing of a probed device that is needed by the
 * uclass.
 *
 * @dev:	Pointer to the device
 * #return 0 on success, -ve on error
 */
int uclass_post_probe_device(struct udevice *dev);

/**
 * uclass_pre_remove_device() - Handle a device which is about to be removed
 *
 * Perform any pre-processing of a device that is about to be removed.
 *
 * @dev:	Pointer to the device
 * #return 0 on success, -ve on error
 */
#ifdef CONFIG_DM_DEVICE_REMOVE
int uclass_pre_remove_device(struct udevice *dev);
#else
static inline int uclass_pre_remove_device(struct udevice *dev) { return 0; }
#endif

/**
 * uclass_find() - Find uclass by its id
 *
 * @id:		Id to serach for
 * @return pointer to uclass, or NULL if not found
 */
struct uclass *uclass_find(enum uclass_id key);

/**
 * uclass_destroy() - Destroy a uclass
 *
 * Destroy a uclass and all its devices
 *
 * @uc: uclass to destroy
 * @return 0 on success, -ve on error
 */
int uclass_destroy(struct uclass *uc);

#endif
