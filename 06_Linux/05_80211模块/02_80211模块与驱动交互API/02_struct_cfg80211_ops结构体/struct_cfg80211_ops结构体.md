# struct cfg80211_ops结构体

>参考文档：
>
>[Release v5.4 · torvalds/linux](https://github.com/torvalds/linux/releases/tag/v5.4)



## struct cfg80211_ops源码

这里列出`Linux 5.4`版本`struct cfg80211_ops`的源码

```c
struct cfg80211_ops {
	int	(*suspend)(struct wiphy *wiphy, struct cfg80211_wowlan *wow);
	int	(*resume)(struct wiphy *wiphy);
	void	(*set_wakeup)(struct wiphy *wiphy, bool enabled);

	struct wireless_dev * (*add_virtual_intf)(struct wiphy *wiphy,
						  const char *name,
						  unsigned char name_assign_type,
						  enum nl80211_iftype type,
						  struct vif_params *params);
	int	(*del_virtual_intf)(struct wiphy *wiphy,
				    struct wireless_dev *wdev);
	int	(*change_virtual_intf)(struct wiphy *wiphy,
				       struct net_device *dev,
				       enum nl80211_iftype type,
				       struct vif_params *params);

	int	(*add_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, bool pairwise, const u8 *mac_addr,
			   struct key_params *params);
	int	(*get_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, bool pairwise, const u8 *mac_addr,
			   void *cookie,
			   void (*callback)(void *cookie, struct key_params*));
	int	(*del_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, bool pairwise, const u8 *mac_addr);
	int	(*set_default_key)(struct wiphy *wiphy,
				   struct net_device *netdev,
				   u8 key_index, bool unicast, bool multicast);
	int	(*set_default_mgmt_key)(struct wiphy *wiphy,
					struct net_device *netdev,
					u8 key_index);

	int	(*start_ap)(struct wiphy *wiphy, struct net_device *dev,
			    struct cfg80211_ap_settings *settings);
	int	(*change_beacon)(struct wiphy *wiphy, struct net_device *dev,
				 struct cfg80211_beacon_data *info);
	int	(*stop_ap)(struct wiphy *wiphy, struct net_device *dev);


	int	(*add_station)(struct wiphy *wiphy, struct net_device *dev,
			       const u8 *mac,
			       struct station_parameters *params);
	int	(*del_station)(struct wiphy *wiphy, struct net_device *dev,
			       struct station_del_parameters *params);
	int	(*change_station)(struct wiphy *wiphy, struct net_device *dev,
				  const u8 *mac,
				  struct station_parameters *params);
	int	(*get_station)(struct wiphy *wiphy, struct net_device *dev,
			       const u8 *mac, struct station_info *sinfo);
	int	(*dump_station)(struct wiphy *wiphy, struct net_device *dev,
				int idx, u8 *mac, struct station_info *sinfo);

	int	(*add_mpath)(struct wiphy *wiphy, struct net_device *dev,
			       const u8 *dst, const u8 *next_hop);
	int	(*del_mpath)(struct wiphy *wiphy, struct net_device *dev,
			       const u8 *dst);
	int	(*change_mpath)(struct wiphy *wiphy, struct net_device *dev,
				  const u8 *dst, const u8 *next_hop);
	int	(*get_mpath)(struct wiphy *wiphy, struct net_device *dev,
			     u8 *dst, u8 *next_hop, struct mpath_info *pinfo);
	int	(*dump_mpath)(struct wiphy *wiphy, struct net_device *dev,
			      int idx, u8 *dst, u8 *next_hop,
			      struct mpath_info *pinfo);
	int	(*get_mpp)(struct wiphy *wiphy, struct net_device *dev,
			   u8 *dst, u8 *mpp, struct mpath_info *pinfo);
	int	(*dump_mpp)(struct wiphy *wiphy, struct net_device *dev,
			    int idx, u8 *dst, u8 *mpp,
			    struct mpath_info *pinfo);
	int	(*get_mesh_config)(struct wiphy *wiphy,
				struct net_device *dev,
				struct mesh_config *conf);
	int	(*update_mesh_config)(struct wiphy *wiphy,
				      struct net_device *dev, u32 mask,
				      const struct mesh_config *nconf);
	int	(*join_mesh)(struct wiphy *wiphy, struct net_device *dev,
			     const struct mesh_config *conf,
			     const struct mesh_setup *setup);
	int	(*leave_mesh)(struct wiphy *wiphy, struct net_device *dev);

	int	(*join_ocb)(struct wiphy *wiphy, struct net_device *dev,
			    struct ocb_setup *setup);
	int	(*leave_ocb)(struct wiphy *wiphy, struct net_device *dev);

	int	(*change_bss)(struct wiphy *wiphy, struct net_device *dev,
			      struct bss_parameters *params);

	int	(*set_txq_params)(struct wiphy *wiphy, struct net_device *dev,
				  struct ieee80211_txq_params *params);

	int	(*libertas_set_mesh_channel)(struct wiphy *wiphy,
					     struct net_device *dev,
					     struct ieee80211_channel *chan);

	int	(*set_monitor_channel)(struct wiphy *wiphy,
				       struct cfg80211_chan_def *chandef);

	int	(*scan)(struct wiphy *wiphy,
			struct cfg80211_scan_request *request);
	void	(*abort_scan)(struct wiphy *wiphy, struct wireless_dev *wdev);

	int	(*auth)(struct wiphy *wiphy, struct net_device *dev,
			struct cfg80211_auth_request *req);
	int	(*assoc)(struct wiphy *wiphy, struct net_device *dev,
			 struct cfg80211_assoc_request *req);
	int	(*deauth)(struct wiphy *wiphy, struct net_device *dev,
			  struct cfg80211_deauth_request *req);
	int	(*disassoc)(struct wiphy *wiphy, struct net_device *dev,
			    struct cfg80211_disassoc_request *req);

	int	(*connect)(struct wiphy *wiphy, struct net_device *dev,
			   struct cfg80211_connect_params *sme);
	int	(*update_connect_params)(struct wiphy *wiphy,
					 struct net_device *dev,
					 struct cfg80211_connect_params *sme,
					 u32 changed);
	int	(*disconnect)(struct wiphy *wiphy, struct net_device *dev,
			      u16 reason_code);

	int	(*join_ibss)(struct wiphy *wiphy, struct net_device *dev,
			     struct cfg80211_ibss_params *params);
	int	(*leave_ibss)(struct wiphy *wiphy, struct net_device *dev);

	int	(*set_mcast_rate)(struct wiphy *wiphy, struct net_device *dev,
				  int rate[NUM_NL80211_BANDS]);

	int	(*set_wiphy_params)(struct wiphy *wiphy, u32 changed);

	int	(*set_tx_power)(struct wiphy *wiphy, struct wireless_dev *wdev,
				enum nl80211_tx_power_setting type, int mbm);
	int	(*get_tx_power)(struct wiphy *wiphy, struct wireless_dev *wdev,
				int *dbm);

	int	(*set_wds_peer)(struct wiphy *wiphy, struct net_device *dev,
				const u8 *addr);

	void	(*rfkill_poll)(struct wiphy *wiphy);

#ifdef CONFIG_NL80211_TESTMODE
	int	(*testmode_cmd)(struct wiphy *wiphy, struct wireless_dev *wdev,
				void *data, int len);
	int	(*testmode_dump)(struct wiphy *wiphy, struct sk_buff *skb,
				 struct netlink_callback *cb,
				 void *data, int len);
#endif

	int	(*set_bitrate_mask)(struct wiphy *wiphy,
				    struct net_device *dev,
				    const u8 *peer,
				    const struct cfg80211_bitrate_mask *mask);

	int	(*dump_survey)(struct wiphy *wiphy, struct net_device *netdev,
			int idx, struct survey_info *info);

	int	(*set_pmksa)(struct wiphy *wiphy, struct net_device *netdev,
			     struct cfg80211_pmksa *pmksa);
	int	(*del_pmksa)(struct wiphy *wiphy, struct net_device *netdev,
			     struct cfg80211_pmksa *pmksa);
	int	(*flush_pmksa)(struct wiphy *wiphy, struct net_device *netdev);

	int	(*remain_on_channel)(struct wiphy *wiphy,
				     struct wireless_dev *wdev,
				     struct ieee80211_channel *chan,
				     unsigned int duration,
				     u64 *cookie);
	int	(*cancel_remain_on_channel)(struct wiphy *wiphy,
					    struct wireless_dev *wdev,
					    u64 cookie);

	int	(*mgmt_tx)(struct wiphy *wiphy, struct wireless_dev *wdev,
			   struct cfg80211_mgmt_tx_params *params,
			   u64 *cookie);
	int	(*mgmt_tx_cancel_wait)(struct wiphy *wiphy,
				       struct wireless_dev *wdev,
				       u64 cookie);

	int	(*set_power_mgmt)(struct wiphy *wiphy, struct net_device *dev,
				  bool enabled, int timeout);

	int	(*set_cqm_rssi_config)(struct wiphy *wiphy,
				       struct net_device *dev,
				       s32 rssi_thold, u32 rssi_hyst);

	int	(*set_cqm_rssi_range_config)(struct wiphy *wiphy,
					     struct net_device *dev,
					     s32 rssi_low, s32 rssi_high);

	int	(*set_cqm_txe_config)(struct wiphy *wiphy,
				      struct net_device *dev,
				      u32 rate, u32 pkts, u32 intvl);

	void	(*mgmt_frame_register)(struct wiphy *wiphy,
				       struct wireless_dev *wdev,
				       u16 frame_type, bool reg);

	int	(*set_antenna)(struct wiphy *wiphy, u32 tx_ant, u32 rx_ant);
	int	(*get_antenna)(struct wiphy *wiphy, u32 *tx_ant, u32 *rx_ant);

	int	(*sched_scan_start)(struct wiphy *wiphy,
				struct net_device *dev,
				struct cfg80211_sched_scan_request *request);
	int	(*sched_scan_stop)(struct wiphy *wiphy, struct net_device *dev,
				   u64 reqid);

	int	(*set_rekey_data)(struct wiphy *wiphy, struct net_device *dev,
				  struct cfg80211_gtk_rekey_data *data);

	int	(*tdls_mgmt)(struct wiphy *wiphy, struct net_device *dev,
			     const u8 *peer, u8 action_code,  u8 dialog_token,
			     u16 status_code, u32 peer_capability,
			     bool initiator, const u8 *buf, size_t len);
	int	(*tdls_oper)(struct wiphy *wiphy, struct net_device *dev,
			     const u8 *peer, enum nl80211_tdls_operation oper);

	int	(*probe_client)(struct wiphy *wiphy, struct net_device *dev,
				const u8 *peer, u64 *cookie);

	int	(*set_noack_map)(struct wiphy *wiphy,
				  struct net_device *dev,
				  u16 noack_map);

	int	(*get_channel)(struct wiphy *wiphy,
			       struct wireless_dev *wdev,
			       struct cfg80211_chan_def *chandef);

	int	(*start_p2p_device)(struct wiphy *wiphy,
				    struct wireless_dev *wdev);
	void	(*stop_p2p_device)(struct wiphy *wiphy,
				   struct wireless_dev *wdev);

	int	(*set_mac_acl)(struct wiphy *wiphy, struct net_device *dev,
			       const struct cfg80211_acl_data *params);

	int	(*start_radar_detection)(struct wiphy *wiphy,
					 struct net_device *dev,
					 struct cfg80211_chan_def *chandef,
					 u32 cac_time_ms);
	int	(*update_ft_ies)(struct wiphy *wiphy, struct net_device *dev,
				 struct cfg80211_update_ft_ies_params *ftie);
	int	(*crit_proto_start)(struct wiphy *wiphy,
				    struct wireless_dev *wdev,
				    enum nl80211_crit_proto_id protocol,
				    u16 duration);
	void	(*crit_proto_stop)(struct wiphy *wiphy,
				   struct wireless_dev *wdev);
	int	(*set_coalesce)(struct wiphy *wiphy,
				struct cfg80211_coalesce *coalesce);

	int	(*channel_switch)(struct wiphy *wiphy,
				  struct net_device *dev,
				  struct cfg80211_csa_settings *params);

	int     (*set_qos_map)(struct wiphy *wiphy,
			       struct net_device *dev,
			       struct cfg80211_qos_map *qos_map);

	int	(*set_ap_chanwidth)(struct wiphy *wiphy, struct net_device *dev,
				    struct cfg80211_chan_def *chandef);

	int	(*add_tx_ts)(struct wiphy *wiphy, struct net_device *dev,
			     u8 tsid, const u8 *peer, u8 user_prio,
			     u16 admitted_time);
	int	(*del_tx_ts)(struct wiphy *wiphy, struct net_device *dev,
			     u8 tsid, const u8 *peer);

	int	(*tdls_channel_switch)(struct wiphy *wiphy,
				       struct net_device *dev,
				       const u8 *addr, u8 oper_class,
				       struct cfg80211_chan_def *chandef);
	void	(*tdls_cancel_channel_switch)(struct wiphy *wiphy,
					      struct net_device *dev,
					      const u8 *addr);
	int	(*start_nan)(struct wiphy *wiphy, struct wireless_dev *wdev,
			     struct cfg80211_nan_conf *conf);
	void	(*stop_nan)(struct wiphy *wiphy, struct wireless_dev *wdev);
	int	(*add_nan_func)(struct wiphy *wiphy, struct wireless_dev *wdev,
				struct cfg80211_nan_func *nan_func);
	void	(*del_nan_func)(struct wiphy *wiphy, struct wireless_dev *wdev,
			       u64 cookie);
	int	(*nan_change_conf)(struct wiphy *wiphy,
				   struct wireless_dev *wdev,
				   struct cfg80211_nan_conf *conf,
				   u32 changes);

	int	(*set_multicast_to_unicast)(struct wiphy *wiphy,
					    struct net_device *dev,
					    const bool enabled);

	int	(*get_txq_stats)(struct wiphy *wiphy,
				 struct wireless_dev *wdev,
				 struct cfg80211_txq_stats *txqstats);

	int	(*set_pmk)(struct wiphy *wiphy, struct net_device *dev,
			   const struct cfg80211_pmk_conf *conf);
	int	(*del_pmk)(struct wiphy *wiphy, struct net_device *dev,
			   const u8 *aa);
	int     (*external_auth)(struct wiphy *wiphy, struct net_device *dev,
				 struct cfg80211_external_auth_params *params);

	int	(*tx_control_port)(struct wiphy *wiphy,
				   struct net_device *dev,
				   const u8 *buf, size_t len,
				   const u8 *dest, const __be16 proto,
				   const bool noencrypt);

	int	(*get_ftm_responder_stats)(struct wiphy *wiphy,
				struct net_device *dev,
				struct cfg80211_ftm_responder_stats *ftm_stats);

	int	(*start_pmsr)(struct wiphy *wiphy, struct wireless_dev *wdev,
			      struct cfg80211_pmsr_request *request);
	void	(*abort_pmsr)(struct wiphy *wiphy, struct wireless_dev *wdev,
			      struct cfg80211_pmsr_request *request);
	int	(*update_owe_info)(struct wiphy *wiphy, struct net_device *dev,
				   struct cfg80211_update_owe_info *owe_info);
	int	(*probe_mesh_link)(struct wiphy *wiphy, struct net_device *dev,
				   const u8 *buf, size_t len);
};
```



## 创建wiphy设备并注册操作集

驱动模块初始化阶段会调用`wiphy_new`接口将提前注册好的`struct cfg80211_ops`结构体传入并注册操作集并返回`struct wiphy *`的返回值

`wiphy_new`函数中会创建一个`struct cfg80211_registered_device`类型的指针，将传入的`ops`存入。这里使用`kzalloc`申请了一块内存，后续流程中就可以使用`container_of`来通过其成员变量获取结构体的指针

```c
struct wiphy *wiphy_new_nm(const struct cfg80211_ops *ops, int sizeof_priv,
			   const char *requested_name)
{
	struct cfg80211_registered_device *rdev;
	int alloc_size;

	alloc_size = sizeof(*rdev) + sizeof_priv;
	rdev = kzalloc(alloc_size, GFP_KERNEL);

	rdev->ops = ops;

	return &rdev->wiphy;
}
EXPORT_SYMBOL(wiphy_new_nm);
```



## ops->suspend接口

**源码如下：**

```c
/*
 * @suspend: wiphy device needs to be suspended. The variable @wow will
 *  be %NULL or contain the enabled Wake-on-Wireless triggers that are
 *  configured for the device.
 */
struct cfg80211_ops {
    ...
	int	(*suspend)(struct wiphy *wiphy, struct cfg80211_wowlan *wow);
	...
}
```

**函数作用：**

当系统进入睡眠/挂起状态时，这个函数指针会被调用来：

1. **配置唤醒功能** - 设置 `WoWLAN` (`Wake-on-WLAN`) 参数
2. **保存设备状态** - 保存当前 `Wi-Fi` 设备的配置和状态
3. **准备低功耗模式** - 让 `Wi-Fi` 硬件进入合适的睡眠状态

**参数说明：**

```c
int (*suspend)(struct wiphy *wiphy, struct cfg80211_wowlan *wow);
```

- **`wiphy`**: 表示无线物理设备的结构体，包含设备的能力和配置信息
- **`wow`**: `WoWLAN` (`Wake-on-WLAN`) 配置结构体，定义了哪些事件可以唤醒设备
- **返回值**: `int` 类型，`0` 表示成功，负值表示错误

**典型使用场景：**

- 笔记本电脑合盖时保持`Wi-Fi`连接
- 移动设备进入睡眠模式但仍能接收重要网络数据
- 设置特定的网络包模式来唤醒设备

这是现代设备电源管理的重要组成部分，确保在节能的同时保持网络连接的可用性

## ops->resume接口

**源码如下：**

```c
/*
 * @resume: wiphy device needs to be resumed
 */
struct cfg80211_ops {
    ...
	int	(*resume)(struct wiphy *wiphy);
	...
}
```

**函数作用：**

当系统从睡眠/挂起状态唤醒时，这个函数指针会被调用来：

1. **恢复设备状态** - 将 `Wi-Fi` 硬件从低功耗模式唤醒
2. **重新初始化连接** - 恢复之前的网络连接和配置
3. **处理唤醒事件** - 检查是什么事件导致了设备唤醒
4. **同步网络状态** - 更新连接状态、信号强度等信息

**函数签名：**

```c
int (*resume)(struct wiphy *wiphy);
```

- **`wiphy`**: 表示无线物理设备的结构体
- **返回值**: `int` 类型，0 表示成功，负值表示错误

**与 suspend 的关系：**

```c
// 挂起时调用
int (*suspend)(struct wiphy *wiphy, struct cfg80211_wowlan *wow);
// 恢复时调用 
int (*resume)(struct wiphy *wiphy);
```

这两个函数形成完整的电源管理周期：

- `suspend` → 准备睡眠，配置唤醒条件
- `resume` → 从睡眠中恢复，重新激活设备

**使用场景：**

- 笔记本从睡眠模式唤醒后恢复 `Wi-Fi` 连接
- 手机接收到重要网络数据后唤醒并处理
- 系统定时唤醒进行网络同步

这个函数确保设备能够`seamlessly`地从低功耗状态恢复到正常工作状态，为用户提供良好的使用体验

## ops->set_wakeup接口

**源码如下：**

```c
/*
 * @set_wakeup: Called when WoWLAN is enabled/disabled, use this callback
 *	to call device_set_wakeup_enable() to enable/disable wakeup from
 *	the device.
 */
struct cfg80211_ops {
    ...
	void (*set_wakeup)(struct wiphy *wiphy, bool enabled);
	...
}
```

**函数作用：**

这个函数主要用于：

1. **启用/禁用唤醒功能** - 动态控制设备是否具备从睡眠状态唤醒系统的能力
2. **电源管理优化** - 在不需要网络唤醒时完全关闭相关功能以节省电力
3. **用户配置控制** - 响应用户或系统策略对唤醒功能的设置

**函数签名：**

```c
void (*set_wakeup)(struct wiphy *wiphy, bool enabled);
```

- **`wiphy`**: 无线物理设备结构体
- **`enabled`**: 布尔值，`true` 启用唤醒功能，`false` 禁用
- **返回值**: `void`，无返回值

**与其他电源管理函数的关系：**

```c
// 电源管理三兄弟
int (*suspend)(struct wiphy *wiphy, struct cfg80211_wowlan *wow); // 挂起时配置
int (*resume)(struct wiphy *wiphy);                // 恢复时处理
void (*set_wakeup)(struct wiphy *wiphy, bool enabled);       // 控制唤醒开关
```

**典型使用场景：**

1. **用户手动控制**

    用户通过系统设置禁用网络唤醒

2. **电池优化**

    /电池电量低时自动禁用唤醒功能

3. **网络策略**

    连接到不信任网络时禁用唤醒

**关键区别：**

与 `suspend` 函数不同：

- `suspend`: 配置**如何**唤醒（设置具体的唤醒条件）
- `set_wakeup`: 控制**是否**允许唤醒（总开关）

这个函数提供了更细粒度的电源管理控制，让系统能够根据实际需求动态调整设备的唤醒能力

## ops->add_virtual_intf接口

**源码如下：**

```c
/*
 * @add_virtual_intf: create a new virtual interface with the given name,
 *	must set the struct wireless_dev's iftype. Beware: You must create
 *	the new netdev in the wiphy's network namespace! Returns the struct
 *	wireless_dev, or an ERR_PTR. For P2P device wdevs, the driver must
 *	also set the address member in the wdev.
 */
struct cfg80211_ops {
    ...
	struct wireless_dev * (*add_virtual_intf)(struct wiphy *wiphy,
						  const char *name,
						  unsigned char name_assign_type,
						  enum nl80211_iftype type,
						  struct vif_params *params);
	...
}
```

**函数作用：**

这个函数主要用于：

1. **创建虚拟Wi-Fi接口** - 在单个物理Wi-Fi设备上创建多个逻辑接口
2. **支持多种工作模式** - 同时支持STA、AP、P2P等不同模式
3. **网络虚拟化** - 实现一个物理设备承载多个网络功能

**参数说明：**

- **`wiphy`**: 物理Wi-Fi设备结构体
- **`name`**: 新接口的名称（如 "wlan0", "p2p0"）
- **`name_assign_type`**: 名称分配类型（内核分配 vs 用户指定）
- **`type`**: 接口类型枚举值
- **`params`**: 虚拟接口的配置参数
- **返回值**: 成功时返回 `wireless_dev` 结构体指针，失败时返回错误码

**典型使用场景：**

1. **同时AP和STA模式**

    创建`AP`接口，创建`STA`接口

2. **P2P功能**

    创建`P2P`设备接口

3. **网络监控**

    创建监控接口

**应用优势：**

- **资源复用**: 一个物理设备支持多种功能
- **灵活性**: 动态创建不同类型的接口
- **成本效益**: 减少硬件需求
- **功能并行**: 同时进行不同的网络操作

这个函数是现代`Wi-Fi`设备实现多功能和虚拟化的核心机制，使得单个`Wi-Fi`芯片可以同时承担多种网络角色

## ops->del_virtual_intf接口

**源码如下：**

```c
/*
 * @del_virtual_intf: remove the virtual interface
 */
struct cfg80211_ops {
    ...
	int	(*del_virtual_intf)(struct wiphy *wiphy,
				    struct wireless_dev *wdev);
	...
}
```

**函数作用：**

这个函数主要用于：

1. **删除虚拟Wi-Fi接口** - 移除之前创建的虚拟接口
2. **资源清理** - 释放接口占用的内存和硬件资源
3. **网络配置管理** - 动态调整设备的网络配置

**参数说明：**

- **`wiphy`**: 物理`Wi-Fi`设备结构体
- **`wdev`**: 要删除的虚拟接口的 `wireless_dev` 结构体指针
- **返回值**: `int` 类型，`0` 表示成功，负值表示错误

**与add_virtual_intf的关系：**

```c
// 创建虚拟接口
struct wireless_dev * (*add_virtual_intf)(...);
// 删除虚拟接口 
int (*del_virtual_intf)(struct wiphy *wiphy, struct wireless_dev *wdev);
```

这两个函数形成完整的接口生命周期管理：

- `add_virtual_intf` → 创建新的虚拟接口
- `del_virtual_intf` → 删除现有的虚拟接口

**典型使用场景：**

1. **删除AP接口**

    删除之前创建的`AP`接口

2. **清理P2P接口**

    删除`P2P`接口

3. **动态配置调整**

    先删除现有接口，再创建新配置的接口

这个函数是`Wi-Fi`设备动态配置管理的重要组成部分，确保系统能够灵活地创建和销毁虚拟网络接口。

## ops->change_virtual_intf接口

**源码如下：**

```c
/*
 * @change_virtual_intf: change type/configuration of virtual interface,
 *	keep the struct wireless_dev's iftype updated.
 */
struct cfg80211_ops {
    ...
	int	(*change_virtual_intf)(struct wiphy *wiphy,
				       struct net_device *dev,
				       enum nl80211_iftype type,
				       struct vif_params *params);
	...
}
```

**函数作用：**

这个函数指针允许用户空间程序（如 `iw` 工具）动态地改变无线网络接口的工作模式，比如将接口从 `Station` 模式切换到 `AP` 模式，或者切换到 `Monitor` 模式等。

**参数说明：**

- **`wiphy`**: 指向 `wiphy` 结构体的指针，代表物理无线设备
- **`dev`**: 指向 `net_device` 结构体的指针，代表要修改的网络接口
- **`type`**: `nl80211_iftype` 枚举值，指定新的接口类型
- **`params`**: 指向 `vif_params` 结构体的指针，包含额外的配置参数

**典型使用场景：**

1. **WiFi热点创建**: 将接口从`Station`模式切换到`AP`模式

    ```shell
    iw dev wlan0 set type __ap
    ```

2. **网络监控**: 切换到`Monitor`模式进行数据包捕获

    ```shell
    iw dev wlan0 set type monitor
    ```

3. **Mesh网络**: 配置设备作为`Mesh`网络节点

    ```shell
    iw dev wlan0 set type mp
    ```

这个函数是无线驱动程序必须实现的核心回调之一，它使得`Linux`系统能够灵活地配置无线网络接口的工作模式

## ops->add_key接口

**源码如下：**

```c
/*
 * @add_key: add a key with the given parameters. @mac_addr will be %NULL
 *	when adding a group key.
 */
struct cfg80211_ops {
    ...
	int	(*add_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, bool pairwise, const u8 *mac_addr,
			   struct key_params *params);
	...
}
```

**函数作用：**

这个函数指针用于向无线网络接口添加加密密钥，支持各种无线安全协议（如 `WEP`、`WPA/WPA2`、`WPA3`）的密钥管理

**参数说明：**

- **`wiphy`**: 指向物理无线设备结构体，标识具体的无线硬件
- **`netdev`**: 指向网络设备结构体，表示要配置密钥的网络接口
- **`key_index`**: 密钥索引（`0-3`），用于标识不同的密钥槽位
- **`pairwise`**: 布尔值，区分密钥类型：
    - `true`: 单播密钥（`Pairwise Key`，用于点对点通信）
    - `false`: 组播密钥（`Group Key`，用于广播/组播通信）
- **`mac_addr`**: `MAC`地址指针
    - 对于单播密钥：指向对等设备的`MAC`地址
    - 对于组播密钥：通常为`NULL`
- **`params`**: 指向 `key_params` 结构体，包含密钥的详细信息

**key_params结构体：**

```c
struct key_params {
	const u8 *key;
	const u8 *seq;
	int key_len;
	int seq_len;
	u32 cipher;
	enum nl80211_key_mode mode;
};
```

**典型使用场景：**

* `WPA2-PSK` 连接

    当客户端连接到使用`WPA2-PSK`的`AP`时：

    用户空间工具(如`wpa_supplicant`)调用该函数

* `WEP`网络连接：

    * 添加`WEP`密钥

        ```c
        iw dev wlan0 connect "MyNetwork" key 0:1234567890
        ```

* 企业级`WPA2`(`802.1X`)：

    *  添加单播密钥(`PTK`)

    * 添加组播密钥(`GTK`)

**常见加密算法：**

- **`WLAN_CIPHER_SUITE_WEP40`**: `WEP 40`位
- **`WLAN_CIPHER_SUITE_WEP104`**: `WEP 104`位
- **`WLAN_CIPHER_SUITE_TKIP`**: `TKIP` (`WPA`)
- **`WLAN_CIPHER_SUITE_CCMP`**: `AES-CCMP` (`WPA2`)
- **`WLAN_CIPHER_SUITE_GCMP`**: `AES-GCMP` (`WPA3`)

这个函数是无线网络安全的基础，所有的加密通信都依赖于正确的密钥管理

## ops->get_key接口

**源码如下：**

```c
/*
 * @get_key: get information about the key with the given parameters.
 *	@mac_addr will be %NULL when requesting information for a group
 *	key. All pointers given to the @callback function need not be valid
 *	after it returns. This function should return an error if it is
 *	not possible to retrieve the key, -ENOENT if it doesn't exist.
 */
struct cfg80211_ops {
    ...
	int	(*get_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, bool pairwise, const u8 *mac_addr,
			   void *cookie,
			   void (*callback)(void *cookie, struct key_params*));
	...
}
```

**函数作用：**

这个函数指针用于从无线网络接口获取已配置的加密密钥信息，主要用于密钥状态查询、调试和管理目的。与 `add_key` 相对应，它提供了密钥的读取功能

**参数说明：**

- **`wiphy`**: 指向物理无线设备结构体
- **`netdev`**: 指向要查询密钥的网络接口
- **`key_index`**: 要查询的密钥索引（`0-3`）
- **`pairwise`**: 密钥类型标识
    - `true`: 查询单播密钥（`Pairwise Key`）
    - `false`: 查询组播密钥（`Group Key`）
- **`mac_addr`**: 目标`MAC`地址
    - 单播密钥：对等设备`MAC`地址
    - 组播密钥：通常为`NULL`
- **`cookie`**: 用户定义的上下文数据，会传递给回调函数
- **`callback`**: 回调函数指针，用于异步返回密钥信息

**典型使用场景：**

1. 密钥状态查询

     查询当前连接的PTK状态

2. 调试和诊断

​	遍历所有密钥槽位

3. 密钥同步检查

​	在密钥重协商后验证密钥是否正确安装

**安全考虑：**

出于安全原因，大多数驱动程序实现中：

- 不会返回实际的密钥数据（`key_params->key` 通常为`NULL`）
- 主要返回密钥的元数据信息：
    - 密钥长度
    - 加密算法类型
    - 序列号信息
    - 密钥是否存在的状态

**错误处理：**

- **成功**: 返回`0`，通过回调函数返回密钥信息
- **失败**: 返回负错误码，回调函数参数为`NULL`

这个函数主要用于系统管理和调试，在正常的数据传输过程中很少使用

## ops->del_key接口

**源码如下：**

```c
/*
 * @del_key: remove a key given the @mac_addr (%NULL for a group key)
 *	and @key_index, return -ENOENT if the key doesn't exist.
 */
struct cfg80211_ops {
    ...
	int	(*del_key)(struct wiphy *wiphy, struct net_device *netdev,
			   u8 key_index, bool pairwise, const u8 *mac_addr);
	...
}
```

**函数作用：**

这个函数指针用于从无线网络接口删除指定的加密密钥，通常在断开连接、密钥过期或安全策略变更时调用。它与 `add_key` 配对使用，提供完整的密钥生命周期管理

**参数说明：**

- **`wiphy`**: 指向物理无线设备结构体，标识具体的无线硬件
- **`netdev`**: 指向要删除密钥的网络接口设备
- **`key_index`**: 要删除的密钥索引（`0-3`），指定密钥槽位
- **`pairwise`**: 密钥类型标识符
    - `true`: 删除单播密钥（`Pairwise Key PTK`）
    - `false`: 删除组播密钥（`Group Key GTK`）
- **`mac_addr`**: 目标设备`MAC`地址
    - 单播密钥：对等设备的`MAC`地址
    - 组播密钥：通常为`NULL`或广播地址

**典型使用场景：**

* **断开WiFi连接**

    断开连接时清理所有相关密钥

* **密钥重协商**

    `WPA2`密钥更新过程中删除旧密钥

* **AP模式客户端离开**

    `AP`模式下，当客户端断开连接时

* 安全策略变更

    从`WEP`切换到`WPA`时清理所有`WEP`密钥

**密钥删除的时机：**

* 正常流程
    * **连接断开**: 主动或被动断开WiFi连接
    * **密钥过期**: 根据安全策略定期更新密钥
    * **接口模式切换**: 改变接口工作模式前清理密钥
    * **错误恢复**: 密钥协商失败后的清理

* 紧急情况
    * **安全事件**: 检测到攻击时立即删除密钥
    * **硬件重置**: 设备重启或重置前的清理
    * **驱动卸载**: 模块卸载时的资源清理

**与其他函数的关系：**

```c
// 典型的密钥管理流程
add_key()    	  // 添加密钥
↓
get_key()   	  // 查询密钥状态
↓
set_default_key() // 设置默认密钥
↓
del_key()    	  // 删除密钥
```

这个函数是无线网络安全管理的关键组件，确保密钥的及时清理和系统安全

## ops->set_default_key接口

**源码如下：**

```c
/*
 * @set_default_key: set the default key on an interface
 */
struct cfg80211_ops {
    ...
	int	(*set_default_key)(struct wiphy *wiphy,
				   struct net_device *netdev,
				   u8 key_index, bool unicast, bool multicast);
	...
}
```

**函数作用：**

这个函数指针用于指定无线网络接口的默认传输密钥，主要作用是告诉硬件在发送单播和/或组播数据包时应该使用哪个密钥槽位的密钥进行加密

**参数说明：**

- **`wiphy`**: 指向物理无线设备结构体
- **`netdev`**: 指向要配置的网络接口
- **`key_index`**: 密钥索引（0-3），指定作为默认密钥的槽位
- **`unicast`**: 布尔值，指定该密钥是否用于单播数据传输
    - `true`: 将此密钥设为单播数据的默认密钥
    - `false`: 不用于单播传输
- **`multicast`**: 布尔值，指定该密钥是否用于组播/广播数据传输
    - `true`: 将此密钥设为组播/广播数据的默认密钥
    - `false`: 不用于组播/广播传输

**典型使用场景：**

* **WEP网络配置**

    `WEP`网络中设置默认密钥

* **WPA/WPA2网络的GTK设置**

    在`WPA2`网络中，设置`GTK`作为组播传输的默认密钥

* **AP**模式密钥配置

    `AP`模式下配置广播密钥，通常在`start_ap`后设置

**调用时序：**

```c
// 典型的密钥配置流程
add_key(wiphy, netdev, 0, false, NULL, &key_params);   // 添加密钥
↓
set_default_key(wiphy, netdev, 0, false, true);     // 设置为默认
↓
// 开始正常数据传输，硬件自动使用默认密钥加密
```

这个函数是无线网络数据加密传输的关键环节，确保数据包能够使用正确的密钥进行加密