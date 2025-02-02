//
//  REGISTER.H  Register map include file
//

#ifndef _A60808_REGISTER_H_
#define _A60808_REGISTER_H_

//DRAMC Chip 
#define fcA60808 0
#define fcMT6595 1
//#define fcFOR_CHIP_ID fcA60808
#define fcFOR_CHIP_ID fcMT6595
//#define fcA60808_6595_TEST

//Defined for DRAM FT
//#define DDR_FT_LOAD_BOARD

// MEMPLL
// seal ring or cts mode
//#define fcMEMPLL_SEAL_RING

// DDR Type
#define DDRTYPE_LPDDR3
//#define DDRTYPE_DDR3

//Feature option
#define MT6795_MODIFICATION	//** Should be enabled after bring-up.
#ifdef MT6795_MODIFICATION
//#define DISABLE_PERBANK_REFRESH		// After per-bank refresh finish test, remove this.
#define PBREF_DISBYREFNUM 		// E3 new feature. Per-bank refresh will be disabled when refresh queue count > R_DMDISREFNUM[2:0]
#define SREF_HW_LOSS_ECO		// E3 new feature. Solve HW SREF loss when SREF_HW and special command conflict.
#define STBCALDIS_HW_ENABLE	// E3 new feature. DRAMC check SPM STBCALDIS_HW for HW gating disable.
//#define STBCALDIS_HW_DISABLE
#define PLLPHASE_CAL_ONEPLL
#else
#define DISABLE_PERBANK_REFRESH
#endif
#define E2_MODIFICATION
#define fcNEW_PLL_PHASE_CALIB
#define fcNEW_PLL_GPPHASE_CALIB
#define DUAL_RANKS
//#define SINGLE_CHANNEL_ENABLE	// Boot-up debugging.
#ifdef E2_MODIFICATION
#define TEMP_SENSOR_ENABLE		// E3 : Need to enable later
#endif
//#define SBR						// EMI same bank reorder. Edward?? : Wait Yi-Chih to reply if this needs to be enabled.
//#define PBC_MASK				// EMI pbc_mask not reference reorder_mask 
#define ZQCS_ENABLE			// E3 : Need to enable later
//#define ZQCS_DUAL
#define EMI_OPTIMIZATION
//#define RWSPLIT_ENABLE
//#define MATYPE_ADAPTATION
#define LPDDR_MAX_DRIVE
//#define LPDDR_MIN_DRIVE
#define NEW_TX_DELAY_For_WL
//#define RX_DUTY_CALIBRATION
// CLKTDN=1. Extend clk low duty.
#define CLKTDN_ENABLE
#define WL_CLKADJUST
#define SPM_CONTROL_AFTERK
#define HW_GATING
#define DISABLE_DRVREF
//#define DFS_HOPPING_MAX			25		// This option should be only for internal use which will swith PLL to 1600Mbps when frequency hopping is not supported.
//#define SLT_FH_FINETUNE_TH	18		// This threshold is for SLT to screen the chip with frequency hopping capability.
#define DFS_HOPPING_FINETUNE_MIN	18		// This threshold is for MP preloader to avoid the case which SLT result is wrong..
#define DQ_PLUS_90
#define DQ_MINUS_90_IN_LOW_FREQ
//#define CLKCA_PLUS_90_IN_LOW_FREQ

//#define tREFI_DIV4			// If MR4 disable, need to define this.
//#define tREFI_DIV4_MANUAL
//#define tREFI_DIV2
//#define tREFI_DIV2_MANUAL
//#define tREFI_DIV1_MANUAL
//#define tREFI_DIV1_MANUAL_ENABLE
#define REFTHD_ADJUST_0
#define MEMPLL_NEW_POWERON
//#define MEMPLL_INTERNAL_LOOP_TEST
//#define MEMPLL_EXTERNAL_LOOP_TEST
//#define MEMPLLTEST_MPLL_ONOFF
//#define MEMPLLTEST_POWERON_RESET
//#define MEASURE_PLL

//#define FREQ_BY_CHIP
//#define DDR_667
//#define DDR_763
//#define DDR_799	// This is for 1333/800 combination
//#define DDR_800
//#define DDR_814
//#define DDR_838
//#define DDR_1066
//#define DDR_1333
#define DDR_1466
//#define DDR_1600
//#define DDR_1679
//#define DDR_1700
//#define DDR_1780
//#define WL11
//#define DDR_1866
//#define DDR_2000
//#define DDR_2133
//#define DDR_1420
//#define DDR_2400
//#define DDR_1792

#define fcFIX_GATING_ERROR
#ifdef fcFIX_GATING_ERROR
//#define DRAMC_CALIB_DBG			// depend on #define fcFIX_GATING_ERROR
#endif

#define DUAL_FREQ_K						//** Should be enabled after bring-up
//#define TEST_LOWFREQ
//#define TEST_HIGHFREQ_1466_2_1600		// This option is only for testing in ETT.
//#define DUAL_FREQ_MEMPLL_DISABLE	
//#define DFS_RESET
//#define  DUAL_FREQ_DIFF_VOLTAGE		// 1.125V for high freq and 1.0V for low freq.
#define  DUAL_FREQ_6PLL_MODIFY			// Only 6 PLL setting modification during DDR DVFS
#ifdef DUAL_FREQ_6PLL_MODIFY
#define PLLGRPREG_SIZE	20
#else
#define PLLGRPREG_SIZE	30
#endif
//#define DUAL_FREQ_MPLL_ONOFF
//#define DUAL_FREQ_DIFF_ACTIMING
//#define DUAL_FREQ_DIFF_RLWL
//#define LPDDR3_FREQ_LOW_MR2	0x00170002		// 1160
//#define LPDDR3_FREQ_HIGH_MR2	0x001C0002		// RL14 WL8	1780
#define LPDDR3_FREQ_LOW_MR2	0x00180002		// RL10 WL6 	1333
#define LPDDR3_FREQ_HIGH_MR2	0x001a0002		// RL12 WL6	1600

//#define DUAL_FREQ_LOW_SETTING		407		// 814 Mbps
//#define DUAL_FREQ_HIGH_SETTING		407		// 1466 Mbps

// E3 use : 1466 <-> 800
#ifdef DDR_1466
//#define DUAL_FREQ_LOW_SETTING		533		// 1066 Mbps
#define DUAL_FREQ_LOW_SETTING		407		// 814 Mbps
#define DUAL_FREQ_HIGH_SETTING		733		// 1466 Mbps
#endif

// E3 use : 1333 <-> 799
#ifdef DDR_1333
#define DUAL_FREQ_LOW_SETTING		399		// 799 Mbps
#define DUAL_FREQ_HIGH_SETTING		666		// 1333 Mbps
#endif

// E3 use : 1600 <-> 800
#ifdef DDR_1600
#define DUAL_FREQ_LOW_SETTING		400		// 800 Mbps
#define DUAL_FREQ_HIGH_SETTING		800		// 1600 Mbps
#endif

// E3 use : 1679 <-> 763
#ifdef DDR_1679
//#define DUAL_FREQ_LOW_SETTING		533		
#define DUAL_FREQ_LOW_SETTING			381	
#define DUAL_FREQ_HIGH_SETTING			839		
#endif


#define HOPPINGFAIL_SW_1600		// Internal use??

#ifdef DUAL_FREQ_K
	#ifndef TEST_LOWFREQ
	#define DUAL_FREQ_TEST		//** need to be disable in preloader
	#endif
#define FH_FINETUNE_CENTER_ADJUST	68
#endif

#define CA_WR_ENABLE
#define SPM_MODIFY
//#define CLK_UNGATING
#define ENABLE_DDYPHY_GATING
//#define DUMMYPAD_PD_DISABLE
//#define DRAMCLK_NO_GATING
//#define COMBPHY_GATING_SAME_DDRPHY
//#define ENABLE_05PHY_CMDCG
#define CKE_EXTEND_1T
#define FINETUNE_CENTER
//#define TCPDED_EXTEND
#define CKE_CS_DLY_SETTING
//#define CHA_MEMPLL3_DISABLE
#ifdef E2_MODIFICATION
//#define CHB_MEMPLL3_PHY05_MEMPLL4_DISABLE
#endif
#define SBSPHY_BYPASS
#define DDRPIPE_CG_GATING
//#define ESD_VREF_CHK_TH		20		// This option is used to check if ESD causes Vref issue.

//#define CPUTEST
//#define TA2_STRESS
#define XTALK_SSO_STRESS
//#define NEW_WORST_PAT_64

#define TX_ADV_1T		// For DLE advance.

#define DRIVING_DS2_0	7	// DS[2:0] 7->6
//#define MAX_DRIVING

#ifdef WL_CLKADJUST
	#define CATRAINING_STEP	    0
#else
#ifdef fcMEMPLL_SEAL_RING
#define CATRAINING_STEP	    5
#else // CTS
#define CATRAINING_STEP	    0
#endif
#endif

// Memory part
//#define K3QF2F20DM
//#define K3QF6F60MM
//#define EDFA164A2PF
#define H9CKNNNDATMRPR
//#define H9CKNNNCPTMRPR

// Test option
//#define XTALK_SSO_K
//#define WAVEFORM_MEASURE
//#define CS_ADV
//#define FIX_DLE_19
#define DMA_BURST_LEN 	3		// 7:7x8. 3:4x8
//#define DDR_INIT_TIME_PROFILING
//#define VBIASN_02V
//#define EYE_SCAN
//#define ACDIO_TEST
//#define VREF_ADJUST_MENU

//#define PASR_TEST
//#define DPD_TEST
//#define MEMPLL_RESET_TEST
//#define SUSPEND_TEST_DRAMC_SWRST_TEST
#define SUSPEND_TEST

#define SUSPEND_RDATARST_IN_MEMPLL_ENABLE
//#define SUSPEND_DRAMCRST_IN_MEMPLL_ENABLE
//#define SUSPEND_DRAMCRST_IN_MEMPLL_DISABLE
//#define SUSPEND_DRAMCRST_AFTER_SF
//#define SUSPEND_PWR_RST_B
#define DDRPHY_POWER_DOWN
//#define SUSPEND_DISABLE_MR4
//#define HWGW_SYNC_OUTSIDE_SF
#define HWGW_SYNC_INSIDE_SF
//#define SUSPEND_REFRATE_MANUAL


#define READ_COMPARE_TEST

//#define CUSTOMIZE_COARSE_TUNE
//#define SINGLE_RANK_DMA
//#define RANK1_TEST
//#define RELAXED_TIMING
//#define DR_QUICKENDTEST
//#define RELAXED_XRTR2W
//#define RELAXED_XRT
#define MEM_27_GRID
#define VIO18_ADJUST
//#define FTTEST_ZQONLY
#define DMA_ERROR_DISPLAY
#define DMA_ERROR_STOP
//#define DUMMY_READ
//#define DUMMY_READ_AFTER_RESUME
//#define BYPASS_DUMMYPAD
//#define TUNE_RX_TX_FACTORS
//#define WL_TEST
//#define LOOPBACK_TEST
//#define BOUNDARY_UNSTABLE_TEST
//#define DCM_ENABLE
//#define DISABLE_FR_REFRESH
//#define DISABLE_FR_REFRESH_MAX_REFCNT
//#define DISABLE_RW_PENDING
//#define DMA_FIXED_LEN	0x1000
//#define DISABLE_DUALSCHED
//#define DISABLE_PLL_GROUPK
//#define DISABLE_WRITE_LEVELING
//#define DISABLE_05T
//#define DISABLE_VOLTAGE_ADJ

//#define SINGLE_RANK_TEST
#define DUMMYREAD_DMA
//#define STRESS_MR4_ZQCS
//#define DMA_ALE_BLOCK_TEST

#define FREQ_HOPPING_SIM_TEST
//#define FREQ_HOPPING_GW_ADJUST
//#define FREQ_HOPPING_GW_ADJUST_STRESS
//#define FREQ_HOPPING_GCE
//#define TEST_DMABUSY

#ifndef SLT
#define COMBO_MCP
#endif
//#define SUPPORT_DA

/**************************  *********************/
#if fcFOR_CHIP_ID == fcMT6595
#define CH_INFO 30
#define CHA_DRAMCAO_BASE	0x10004000
#define CHA_DDRPHY_BASE	0x1000F000
#define CHA_DRAMCNAO_BASE	0x1020E000
#define CHB_DRAMCAO_BASE	0x10011000
#define CHB_DDRPHY_BASE	0x10012000
#define CHB_DRAMCNAO_BASE	0x10213000
#define DDR_BASE			0x40000000
#define EMI_APB_BASE 		0x10203000

#ifdef K3QF2F20DM

// Samsung CHA(4+4)+CHB(4+4)
#ifdef SINGLE_CHANNEL_ENABLE
//#define 	LPDDR3_EMI_CONA		0x50535056		// Single channel. 15 bits row and 10 bits column. (channel A)
#define 	LPDDR3_EMI_CONA		0x50575056		// Single channel. 15 bits row and 10 bits column. (channel B)
#else
#define 	LPDDR3_EMI_CONA		0x50535057		// Dual channels. 15 bits row and 10 bits column.
#endif
#define	LPDDR3_ACTIM_000		0x77FD474B
#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
#ifdef TCPDED_EXTEND
#define 	LPDDR3_CONF1_004		0x40048403
#else
#define 	LPDDR3_CONF1_004		0x00048403
#endif
#define 	LPDDR3_DDR2CTL_07C	0x000053B1
#define 	LPDDR3_MODE_REG_63	0x0000003F
#define 	LPDDR3_MODE_REG_10	0x00FF000A
#define 	LPDDR3_MODE_REG_1	0x00830001
#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838) || defined(DDR_763) || defined(DDR_799)
#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
#elif defined(DDR_1333)
#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
#elif defined(DDR_1600) || defined(DDR_1466)  || defined(DDR_814) 
#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
#else
  #ifdef WL11
  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
  #else
#define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
#endif
#endif
#define	LPDDR3_MODE_REG_3	0x00020003
#define	LPDDR3_TEST2_3_044	0xBFC00401
#ifdef tREFI_DIV4
#define 	LPDDR3_CONF2_008		0x0300001A
#define	LPDDR3_PD_CTRL_1DC	0xD1183D42
#else
#define 	LPDDR3_CONF2_008		0x0300006C
#define	LPDDR3_PD_CTRL_1DC	0xD1643D42
#endif
#define	LPDDR3_ACTIM1_1E8		0x91001039
#ifdef RELAXED_XRT
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E3
	#define	LPDDR3_RKCFG_110		0x002F77C1
#else
	#ifdef RELAXED_XRTR2W
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E3
	#define	LPDDR3_RKCFG_110		0x002176C1
	#else
        #define	LPDDR3_AC_TIME_05T_1f8	0x000004E3
        #define	LPDDR3_RKCFG_110		0x002156C1
	#endif
#endif	

#define	LPDDR3_TEST2_4_048	0x2701110D
#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000

#elif defined(K3QF6F60MM)

#ifdef SINGLE_CHANNEL_ENABLE
#define 	LPDDR3_EMI_CONA		0xA053A056		// Single channel. 15 bits row and 10 bits column. (channel A)
//#define 	LPDDR3_EMI_CONA		0xA057A056		// Single channel. 15 bits row and 10 bits column. (channel B)
#else
#define 	LPDDR3_EMI_CONA		0xA053A057		// Dual channels. 15 bits row and 10 bits column.
#endif
#define 	LPDDR3_EMI_CONH		0x33330000		// 6Gb per channel/rank		

#ifdef RELAXED_TIMING
#define	LPDDR3_ACTIM_000		0x993E48AE
#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
#ifdef TCPDED_EXTEND
#define 	LPDDR3_CONF1_004		0x40048403
#else
#define 	LPDDR3_CONF1_004		0x00048403
#endif
#define 	LPDDR3_DDR2CTL_07C	0x000064B1
#define 	LPDDR3_MODE_REG_63	0x0000003F
#define 	LPDDR3_MODE_REG_10	0x00FF000A
#define 	LPDDR3_MODE_REG_1	0x00E30001
#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838) || defined(DDR_763)  || defined(DDR_799)
#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
#elif defined(DDR_1333)
#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
#elif defined(DDR_1600) || defined(DDR_1466)  || defined(DDR_814)
#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
#else
  #ifdef WL11
  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
  #else
#define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
#endif
#endif
#define	LPDDR3_MODE_REG_3	0x00020003
#define	LPDDR3_TEST2_3_044	0xBFC50401
#define 	LPDDR3_CONF2_008		0x03000081
#define	LPDDR3_PD_CTRL_1DC	0xD1647342
#define	LPDDR3_ACTIM1_1E8		0x9100256B
#define	LPDDR3_AC_TIME_05T_1f8	0x00001336
#define	LPDDR3_RKCFG_110		0x002156C1
#define	LPDDR3_TEST2_4_048	0x2F01110D
#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000
#else	//RELAXED_TIMING
#define	LPDDR3_ACTIM_000		0x77FD474B
#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
#ifdef TCPDED_EXTEND
#define 	LPDDR3_CONF1_004		0x40048403
#else
#define 	LPDDR3_CONF1_004		0x00048403
#endif
#define 	LPDDR3_DDR2CTL_07C	0x000053B1
#define 	LPDDR3_MODE_REG_63	0x0000003F
#define 	LPDDR3_MODE_REG_10	0x00FF000A
#define 	LPDDR3_MODE_REG_1	0x00830001
#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838) || defined(DDR_763)  || defined(DDR_799)
#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
#elif defined(DDR_1333)
#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
#elif defined(DDR_1600) || defined(DDR_1466)  || defined(DDR_814) 
#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
#else
  #ifdef WL11
  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
  #else
#define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
#endif
#endif
#define	LPDDR3_MODE_REG_3	0x00020003
#define	LPDDR3_TEST2_3_044	0xBFC40401
#ifdef tREFI_DIV4
#define 	LPDDR3_CONF2_008		0x0300001A
#define	LPDDR3_PD_CTRL_1DC	0xD1186142
#else
 	#ifdef tREFI_DIV2
	#define 	LPDDR3_CONF2_008		0x03000036
	#define	LPDDR3_PD_CTRL_1DC	0xD1326142
 	#else
	#define 	LPDDR3_CONF2_008		0x0300006C
        #define	LPDDR3_PD_CTRL_1DC	0xD1646142
        #endif
#endif
#define	LPDDR3_ACTIM1_1E8		0x91001E59
#ifdef RELAXED_XRT
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
	#define	LPDDR3_RKCFG_110		0x002F77C1
	//#define	LPDDR3_AC_TIME_05T_1f8	0x000004E1
	//#define	LPDDR3_RKCFG_110		0x002156C1
#else
        #ifdef RELAXED_XRTR2W
        #define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
        #define	LPDDR3_RKCFG_110		0x002176C1
	//#define	LPDDR3_AC_TIME_05T_1f8	0x000004E1
	//#define	LPDDR3_RKCFG_110		0x002146C1
        //#define	LPDDR3_RKCFG_110		0x002F77C1
        #else
        #define	LPDDR3_AC_TIME_05T_1f8	0x000004E1
	#define	LPDDR3_RKCFG_110		0x002156C1
	#endif
#endif
#define	LPDDR3_TEST2_4_048	0x2701110D
#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000
#endif

#elif defined(EDFA164A2PF)

#ifdef SINGLE_CHANNEL_ENABLE
#define LPDDR3_EMI_CONA		    0x50535056		// Single channel. 15 bits row and 10 bits column. (channel A)
#else
#define LPDDR3_EMI_CONA		    0x50535057		// Dual channels. 14 bits row and 10 bits column.
#endif	

#define	LPDDR3_ACTIM_000		0x77FD474B
#define LPDDR3_GDDR3CTL1_0F4	0x11000000
#ifdef TCPDED_EXTEND
#define 	LPDDR3_CONF1_004		0x40048403
#else
#define LPDDR3_CONF1_004		0x00048403
#endif
#define LPDDR3_DDR2CTL_07C	    0x000053B1
#define LPDDR3_MODE_REG_63	    0x0000003F
#define LPDDR3_MODE_REG_10	    0x00FF000A
#define LPDDR3_MODE_REG_1	    0x00830001
#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838) || defined(DDR_763)  || defined(DDR_799)
#define LPDDR3_MODE_REG_2	    0x00170002		// RL9 WL5
#elif defined(DDR_1333)
#define LPDDR3_MODE_REG_2	    0x00180002		// RL10 WL6
#elif defined(DDR_1600) || defined(DDR_1466)  || defined(DDR_814) 
#define LPDDR3_MODE_REG_2	    0x001A0002		// RL12 WL6
#else
  #ifdef WL11
  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
  #else
#define LPDDR3_MODE_REG_2	    0x001C0002		// RL14 WL8	
#endif
#endif
#define	LPDDR3_MODE_REG_3	    0x00020003
#define	LPDDR3_TEST2_3_044	    0xBFC00401
#ifdef tREFI_DIV4
#define 	LPDDR3_CONF2_008		0x0300001A
#define	LPDDR3_PD_CTRL_1DC	0xD1183D42
#else
#define LPDDR3_CONF2_008		0x0300006C
#define	LPDDR3_PD_CTRL_1DC	    0xD1643D42
#endif
#define	LPDDR3_ACTIM1_1E8		0x91001039
#ifdef RELAXED_XRT
#define	LPDDR3_AC_TIME_05T_1f8	0x020004E3
	#define	LPDDR3_RKCFG_110		0x002F77C1
#else
	#ifdef RELAXED_XRTR2W
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E3
	#define	LPDDR3_RKCFG_110		0x002176C1
	#else
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E3
	#define	LPDDR3_RKCFG_110		0x002156C1
	#endif
#endif
#define	LPDDR3_TEST2_4_048	    0x2701110D
#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000

#elif defined(H9CKNNNDATMRPR) //Hynix 3GB

	#ifdef DISABLE_05T
		#ifdef SINGLE_CHANNEL_ENABLE
		#define 	LPDDR3_EMI_CONA		0xA053A056		// Single channel. 15 bits row and 10 bits column. (channel A)
		#else
		#define 	LPDDR3_EMI_CONA		0xA053A057		// Dual channels. 15 bits row and 10 bits column.
		#endif
		#define 	LPDDR3_EMI_CONH		0x33330000		// 6Gb per channel/rank		

		#define	LPDDR3_ACTIM_000		0x88FD475B
		#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
		#define 	LPDDR3_CONF1_004		0x00048403
		#define 	LPDDR3_DDR2CTL_07C	0x000054B1
		#define 	LPDDR3_MODE_REG_63	0x0000003F
		#define 	LPDDR3_MODE_REG_10	0x00FF000A
		#define 	LPDDR3_MODE_REG_1	0x00830001
		#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838)  || defined(DDR_763)  || defined(DDR_799)
		#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
		#elif defined(DDR_1333)
		#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
		#elif defined(DDR_1600) || defined(DDR_1466) || defined(DDR_814) 
		#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
		#else
		  #ifdef WL11
		  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
		  #else
		  #define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
		  #endif
		#endif
		#define	LPDDR3_MODE_REG_3	0x00020003
		#define	LPDDR3_TEST2_3_044	0xBFC40401
	        #define 	LPDDR3_CONF2_008		0x0300006C
	        #define	LPDDR3_PD_CTRL_1DC	0xD1646142
		#define	LPDDR3_ACTIM1_1E8		0x91001E59
	        #define	LPDDR3_AC_TIME_05T_1f8	0x00000000
	        #define	LPDDR3_RKCFG_110		0x002156C1
		#define	LPDDR3_TEST2_4_048	0x2701110D
		#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000	
	#else	// DISABLE_05T
		#ifdef SINGLE_CHANNEL_ENABLE
		#define 	LPDDR3_EMI_CONA		0xA053A056		// Single channel. 15 bits row and 10 bits column. (channel A)
		#else
		#define 	LPDDR3_EMI_CONA		0xA053A057		// Dual channels. 15 bits row and 10 bits column.
		#endif
		#define 	LPDDR3_EMI_CONH		0x33330000		// 6Gb per channel/rank		

		#define	LPDDR3_ACTIM_000		0x77FD474B
		#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
		#ifdef TCPDED_EXTEND
		#define 	LPDDR3_CONF1_004		0x40048403
		#else
		#define 	LPDDR3_CONF1_004		0x00048403
		#endif
		#define 	LPDDR3_DDR2CTL_07C	0x000053B1
		#define 	LPDDR3_MODE_REG_63	0x0000003F
		#define 	LPDDR3_MODE_REG_10	0x00FF000A
		#define 	LPDDR3_MODE_REG_1	0x00830001
		#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838) || defined(DDR_763)  || defined(DDR_799)
		#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
		#elif defined(DDR_1333)
		#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
		#elif defined(DDR_1600) || defined(DDR_1466)  || defined(DDR_814)
		#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
		#else
		  #ifdef WL11
		  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
		  #else
		  #define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
		  #endif
		#endif
		#define	LPDDR3_MODE_REG_3	0x00020003
		#define	LPDDR3_TEST2_3_044	0xBFC40401
		#ifdef tREFI_DIV4
		#define 	LPDDR3_CONF2_008		0x0300001A
		#define	LPDDR3_PD_CTRL_1DC	0xD1186142
		#else
		 	#ifdef tREFI_DIV2
			#define 	LPDDR3_CONF2_008		0x03000036
			#define	LPDDR3_PD_CTRL_1DC	0xD1326142
			//#define 	LPDDR3_CONF2_008		0x0300001e
			//#define	LPDDR3_PD_CTRL_1DC	0xD11c6142
		 	#else
		        #define 	LPDDR3_CONF2_008		0x0300006C
		        #define	LPDDR3_PD_CTRL_1DC	0xD1646142
		        #endif
		#endif
		#define	LPDDR3_ACTIM1_1E8		0x91001E59
		#ifdef RELAXED_XRT
			#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
			#define	LPDDR3_RKCFG_110		0x002F77C1
		#else
			#ifdef RELAXED_XRTR2W
			#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
			#define	LPDDR3_RKCFG_110		0x002176C1
			#else
		        #define	LPDDR3_AC_TIME_05T_1f8	0x000004E1
		        #define	LPDDR3_RKCFG_110		0x002156C1
		        #endif
		#endif
		#define	LPDDR3_TEST2_4_048	0x2701110D
		#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000
	#endif	//DISABLE_05T
#elif defined(H9CKNNNCPTMRPR)  //Hynix 4GB

#ifdef SINGLE_CHANNEL_ENABLE
#define 	LPDDR3_EMI_CONA		0xA053A056		// Single channel. 15 bits row and 10 bits column. (channel A)
#else
#define 	LPDDR3_EMI_CONA		0xA053A057		// Dual channels. 15 bits row and 10 bits column.
#endif
#define 	LPDDR3_EMI_CONH		0x00000000			

#define     LPDDR3_ACTIM_000		0x77FD474B
#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
#ifdef TCPDED_EXTEND
#define 	LPDDR3_CONF1_004		0x40048403
#else
#define 	LPDDR3_CONF1_004		0x00048403
#endif
#define 	LPDDR3_DDR2CTL_07C	0x000053B1
#define 	LPDDR3_MODE_REG_63	0x0000003F
#define 	LPDDR3_MODE_REG_10	0x00FF000A
#define 	LPDDR3_MODE_REG_1	0x00830001
#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838)  || defined(DDR_763)  || defined(DDR_799)
#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
#elif defined(DDR_1333)
#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
#elif defined(DDR_1600) || defined(DDR_1466) || defined(DDR_814)
#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
#else
  #ifdef WL11
  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
  #else
  #define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
  #endif
#endif
#define	LPDDR3_MODE_REG_3	0x00020003
#define	LPDDR3_TEST2_3_044	0xBFC40401
#ifdef tREFI_DIV4
#define 	LPDDR3_CONF2_008		0x0300001A
#define	LPDDR3_PD_CTRL_1DC	0xD1186142
#else
 	#ifdef tREFI_DIV2
	#define 	LPDDR3_CONF2_008		0x03000036
	#define	LPDDR3_PD_CTRL_1DC	0xD1326142
	//#define 	LPDDR3_CONF2_008		0x0300001e
	//#define	LPDDR3_PD_CTRL_1DC	0xD11c6142
 	#else
        #define 	LPDDR3_CONF2_008		0x0300006C
        #define	LPDDR3_PD_CTRL_1DC	0xD1646142
        #endif
#endif
#define	LPDDR3_ACTIM1_1E8		0x91001E59
#ifdef RELAXED_XRT
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
	#define	LPDDR3_RKCFG_110		0x002F77C1
#else
	#ifdef RELAXED_XRTR2W
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
	#define	LPDDR3_RKCFG_110		0x002176C1
	#else
        #define	LPDDR3_AC_TIME_05T_1f8	0x000004E1
        #define	LPDDR3_RKCFG_110		0x002156C1
        #endif
#endif
#define	LPDDR3_TEST2_4_048	0x2701110D
#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000

#else

// EVB default LPDDR3 part.
#ifdef SINGLE_CHANNEL_ENABLE
//#define 	LPDDR3_EMI_CONA		0x20102016		// Single channel. 15 bits row and 10 bits column. (channel A)
#define 	LPDDR3_EMI_CONA		0x20142016		// Single channel. 15 bits row and 10 bits column. (channel B)
#else
#define 	LPDDR3_EMI_CONA		0x20102017		// Dual channels. 15 bits row and 10 bits column.
#endif
#define	LPDDR3_ACTIM_000		0x77FD474B
#define 	LPDDR3_GDDR3CTL1_0F4	0x11000000
#ifdef TCPDED_EXTEND
#define 	LPDDR3_CONF1_004		0x40048403
#else
#define 	LPDDR3_CONF1_004		0x00048403
#endif
#define 	LPDDR3_DDR2CTL_07C	0x000053B1
#define 	LPDDR3_MODE_REG_63	0x0000003F
#define 	LPDDR3_MODE_REG_10	0x00FF000A
#define 	LPDDR3_MODE_REG_1	0x00830001
#if  defined(DDR_1066) || defined(DDR_800) || defined(DDR_838) || defined(DDR_763)  || defined(DDR_799)
#define 	LPDDR3_MODE_REG_2	0x00170002		// RL9 WL5
#elif defined(DDR_1333)
#define 	LPDDR3_MODE_REG_2	0x00180002		// RL10 WL6
#elif defined(DDR_1600) || defined(DDR_1466)  || defined(DDR_814) 
#define 	LPDDR3_MODE_REG_2	0x001A0002		// RL12 WL6
#else
  #ifdef WL11
  #define 	LPDDR3_MODE_REG_2	0x005C0002		// RL14 WL11	
  #else
#define 	LPDDR3_MODE_REG_2	0x001C0002		// RL14 WL8	
#endif
#endif
#define	LPDDR3_MODE_REG_3	0x00020003
#define	LPDDR3_TEST2_3_044	0xBFC40401
#ifdef tREFI_DIV4
#define 	LPDDR3_CONF2_008		0x0300001A
#define	LPDDR3_PD_CTRL_1DC	0xD1186142
#else
 	#ifdef tREFI_DIV2
	#define 	LPDDR3_CONF2_008		0x03000036
	#define	LPDDR3_PD_CTRL_1DC	0xD1326142
	//#define 	LPDDR3_CONF2_008		0x0300001e
	//#define	LPDDR3_PD_CTRL_1DC	0xD11c6142
 	#else
	#define 	LPDDR3_CONF2_008		0x0300006C
#define	LPDDR3_PD_CTRL_1DC	0xD1646142
#endif
#endif
#define	LPDDR3_ACTIM1_1E8		0x91001E59
#ifdef RELAXED_XRT
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
	#define	LPDDR3_RKCFG_110		0x002F77C1
#else
	#ifdef RELAXED_XRTR2W
	#define	LPDDR3_AC_TIME_05T_1f8	0x020004E1
	#define	LPDDR3_RKCFG_110		0x002176C1
	#else
        #define	LPDDR3_AC_TIME_05T_1f8	0x000004E1
        #define	LPDDR3_RKCFG_110		0x002156C1
	#endif
#endif	
#define	LPDDR3_TEST2_4_048	0x2701110D
#define	LPDDR3_MISCTL0_VAL_0fc	0x21000000

#endif




#ifdef SINGLE_CHANNEL_ENABLE
#define 	PCDDR3_EMI_CONA		0x20102016		// Single channel. 15 bits row and 10 bits column.
#else
#define 	PCDDR3_EMI_CONA		0x20102017		// Dual channels. 15 bits row and 10 bits column.
#endif

#endif

#define DRAMC_BASE_ADDRESS 0x20000000
#define REG_BANK_DRAMC_CHA 1
#define POS_BANK_NUM 12
#define XTALK_SSO_LEN	 3396

// PLL registers

// DRAMC registers
#define DRAMC_REG_ACTIM0 0x000
    #define POS_ACTIM0_BL2 15
#define DRAMC_REG_CONF1 0x004
    #define POS_CONF1_BL4 10
    #define POS_CONF1_SELFREF 26
    #define POS_CONF1_TESTLP 27    
#define DRAMC_REG_CONF2 0x008
    #define POS_CONF2_TEST1 29
    #define MASK_CONF2_TE12_ENABLE 0xe0000000
#define DRAMC_REG_PADCTL1 0x00c
#define DRAMC_REG_PADCTL2 0x010
#define DRAMC_REG_PADCTL3 0x014
#define DRAMC_REG_DELDLY1 0x018
#define DRAMC_REG_01C 0x01c
#define DRAMC_REG_DLLCONF 0x028
#define DRAMC_REG_TEST2_0 0x038   // new on MT5399
#define DRAMC_REG_TEST2_1 0x03C
#define DRAMC_REG_TEST2_2 0x040
#define DRAMC_REG_TEST2_3 0x044
    #define POS_TEST2_3_ADVREFEN 30
    #define POS_TEST2_3_TESTAUDPAT 7
    // bit0~3
    #define MASK_TEST2_3_TESTCNT 0x0000000f
    #define POS_TEST2_3_TESTCNT 0
#define DRAMC_REG_TEST2_4 0x048
    // bit0~4
    #define MASK_TEST2_4_TESTAUDINC 0x0000001f
    #define POS_TEST2_4_TESTAUDINC 0
    // bit8~12
    #define MASK_TEST2_4_TESTAUDINIT 0x00001f00
    #define POS_TEST2_4_TESTAUDINIT 8
    // bit 14, 15
    #define POS_TEST2_4_TESTAUDBITINV 14
    #define POS_TEST2_4_TESTAUDMODE 15
    //bit 16
    #define POS_TEST2_4_TESTXTALKPAT 16
#define DRAMC_REG_DDR2CTL 0x07c
    #define MASK_DDR2CTL_DATLAT 0x00000070
    #define POS_DDR2CTL_DTALAT 4
    #define POS_DDR2CTL_WOEN 3
#define DRAMC_REG_084 0x084
#define DRAMC_REG_MRS 0x088
#define DRAMC_REG_CLK1DELAY 0x08c
    #define POS_CLK1DELAY_DQCMD 3
#define DRAMC_REG_IOCTL 0x090
#define DRAMC_REG_DQSIEN 0x094
    #define MASK_DQSIEN_R0DQS0IEN 0x0000007f
    #define MASK_DQSIEN_R0DQS1IEN 0x00007f00
    #define MASK_DQSIEN_R0DQS2IEN 0x007f0000
    #define MASK_DQSIEN_R0DQS3IEN 0x7f000000
    #define POS_DQSIEN_R0DQS0IEN 0
    #define POS_DQSIEN_R0DQS1IEN 8
    #define POS_DQSIEN_R0DQS2IEN 16
    #define POS_DQSIEN_R0DQS3IEN 24
#define DRAMC_REG_R1DQSIEN 0x098
    #define MASK_DQSIEN_R1DQS0IEN 0x0000007f
    #define MASK_DQSIEN_R1DQS1IEN 0x00007f00
    #define MASK_DQSIEN_R1DQS2IEN 0x007f0000
    #define MASK_DQSIEN_R1DQS3IEN 0x7f000000
    #define POS_DQSIEN_R1DQS0IEN 0
    #define POS_DQSIEN_R1DQS1IEN 8
    #define POS_DQSIEN_R1DQS2IEN 16
    #define POS_DQSIEN_R1DQS3IEN 24    
#define DRAMC_REG_IODRV4 0x0b0
    #define MASK_IODRV4_DQDRVN_2 0x00000f00
    #define MASK_IODRV4_DQDRVP_2 0x0000f000
    #define MASK_IODRV4_CMDDRVN_2 0x0f000000
    #define MASK_IODRV4_CMDDRVP_2 0xf0000000
    #define POS_IODRV4_DQDRVN_2 8
    #define POS_IODRV4_DQDRVP_2 12
    #define POS_IODRV4_CMDDRVN_2 24
    #define POS_IODRV4_CMDDRVP_2 28
#define DRAMC_REG_IODRV5 0x0b4
    #define MASK_IODRV5_DQCDRVN 0x00000f00
    #define MASK_IODRV5_DQCDRVP 0x0000f000
    #define MASK_IODRV5_DQCDRVN_2 0x0f000000
    #define MASK_IODRV5_DQCDRVP_2 0xf0000000
    #define POS_IODRV5_DQCDRVN 8
    #define POS_IODRV5_DQCDRVP 12
    #define POS_IODRV5_DQCDRVN_2 24
    #define POS_IODRV5_DQCDRVP_2 28
#define DRAMC_REG_DRVCTL0 0x0b8
    #define MASK_DRVCTL0_DQDRVN 0x00000f00
    #define MASK_DRVCTL0_DQDRVP 0x0000f000
    #define MASK_DRVCTL0_DQSDRVN 0x0f000000
    #define MASK_DRVCTL0_DQSDRVP 0xf0000000
    #define POS_DRVCTL0_DQDRVN 8
    #define POS_DRVCTL0_DQDRVP 12
    #define POS_DRVCTL0_DQSDRVN 24
    #define POS_DRVCTL0_DQSDRVP 28
#define DRAMC_REG_DRVCTL1 0x0bc
    #define MASK_DRVCTL1_CMDDRVN 0x00000f00
    #define MASK_DRVCTL1_CMDDRVP 0x0000f000
    #define MASK_DRVCTL1_CLKDRVN 0x0f000000
    #define MASK_DRVCTL1_CLKDRVP 0xf0000000
    #define POS_DRVCTL1_CMDDRVN 8
    #define POS_DRVCTL1_CMDDRVP 12
    #define POS_DRVCTL1_CLKDRVN 24
    #define POS_DRVCTL1_CLKDRVP 28
#define DRAMC_REG_MCKDLY 0x0d8
    //bit 23
    #define POS_MCKDLY_FIXODT 23
    //bit 12~15
    #define POS_MCKDLY_FIXDQIEN 12
    #define MASK_MCKDLY_FIXDQIEN 0x0000f000
    // bit 10,11
    #define POS_MCKDLY_DQIENQKEND 10
    #define MASK_MCKDLY_DQIENQKEND 0x00000c00
#define DRAMC_REG_DQSCTL0 0x0dc
#define DRAMC_REG_DQSCTL1 0x0e0
    #define POS_DQSCTL1_DQSIENMODE 28
    #define MASK_DQSCTL1_DQSINCTL 0x0f000000   // for A60808, 4 bits totally
    #define POS_DQSCTL1_DQSINCTL 24
    #define POS_DQSCTL1_DQS2CTL 0   // [3:0] A60808: HW run-time DATLAT
    #define MASK_DQSCTL1_DQS2CTL 0x0000000f
#define DRAMC_REG_PADCTL4 0x0e4
    #define MASK_PADCTL4_DATLAT3 0x00000010
    #define POS_PADCTL4_DATLAT3 4
    #define POS_PADCTL4_CKEFIXON 2
#define DRAMC_REG_PHYCTL1 0x0f0
    #define MASK_PHYCTL1_DATLAT4 0x02000000
    #define POS_PHYCTL1_DATLAT4 25
    #define POS_PHYCTL1_PHYRST 28
#define DRAMC_REG_GDDR3CTL1 0x0f4
    #define POS_GDDR3CTL1_DQMSWAP 31
    #define POS_GDDR3CTL1_RDATRST 25
    #define POS_GDDR3CTL1_BKSWAP 20
#define DRAMC_REG_PADCTL7 0x0f8
#define DRAMC_REG_MISCTL0 0x0fc
#define DRAMC_REG_OCDK 0x100
    #define POS_OCDK_DRVREF 24
#define DRAMC_REG_RKCFG 0x110
    #define POS_RKCFG_PBREF_DISBYRATE 6
    #define POS_RKCFG_WDATKEY64 29   //32-bit channel it is 0, for 16-bit or asym DRAM it is 1
#define DRAMC_REG_CKPHDET 0x114
#define DRAMC_REG_DQSCTL2 0x118
    #define MASK_DQSCTL2_DQSINCTL 0x0000000f
    #define POS_DQSCTL2_DQSINCTL 0
#define DRAMC_REG_DQ_DQS01_SEL 0x120
#define DRAMC_REG_DQSGCTL 0x124
    #define MASK_DQSGCTL_R0DQSG_COARSE_DLY_COM0 0x00000003
    #define MASK_DQSGCTL_R1DQSG_COARSE_DLY_COM0 0x0000000C
    #define MASK_DQSGCTL_R0DQSG_COARSE_DLY_COM1 0x00000030
    #define MASK_DQSGCTL_R1DQSG_COARSE_DLY_COM1 0x000000C0
    #define POS_DQSGCTL_R0DQSG_COARSE_DLY_COM0 0
    #define POS_DQSGCTL_R1DQSG_COARSE_DLY_COM0 2
    #define POS_DQSGCTL_R0DQSG_COARSE_DLY_COM1 4
    #define POS_DQSGCTL_R1DQSG_COARSE_DLY_COM1 6
    #define POS_DQSGCTL_DQSGDUALP 30
#define DRAMC_REG_CLKENCTL 0x130
#define DRAMC_REG_WRLEV 0x13c
    //bit 12
    #define POS_WRLEV_DQS_SEL 12
    #define MASK_WRLEV_DQS_SEL 0x0000f000
    //bit 8
    #define POS_WRLEV_DQS_WLEV 8
    //bit 1~4
    #define POS_WRLEV_DQS_Bx_G 1
    #define MASK_WRLEV_DQS_Bx_G 0x0000001e
    //bit 0
    #define POS_WRLEV_WRITE_LEVEL_EN 0
#define DRAMC_REG_PHYCLKDUTY 0x148
    #define POS_PHYCLKDUTY_CMDCLKP0DUTYSEL 28
    #define POS_PHYCLKDUTY_CMDCLKP0DUTYP 18
    #define MASK_PHYCLKDUTY_CMDCLKP0DUTYP 0x000c0000
    #define POS_PHYCLKDUTY_CMDCLKP0DUTYN 16
    #define MASK_PHYCLKDUTY_CMDCLKP0DUTYN 0x00030000
#define DRAMC_REG_158 0x158
#define DRAMC_REG_ARBCTL0 0x168
#define DRAMC_REG_CMDDLY0 0x1a8
#define DRAMC_REG_CMDDLY1 0x1ac
#define DRAMC_REG_CMDDLY2 0x1b0
#define DRAMC_REG_CMDDLY3 0x1b4
#define DRAMC_REG_CMDDLY4 0x1b8
#define DRAMC_REG_DQSCAL0 0x1c0
    #define POS_DQSCAL0_STBCALEN 31
#define DRAMC_REG_IMPCAL 0x1c8
    #define POS_IMPCAL_IMPCALI_EN 0
    #define POS_IMPCAL_IMPCAL_HW 1
    #define POS_IMPCAL_IMP_CALI_ENN 4
    #define POS_IMPCAL_IMP_CALI_ENP 5
    #define POS_IMPCAL_IMPPDN 6
    #define POS_IMPCAL_IMPPDP 7    
    #define POS_IMPCAL_IMPDRVP 8
    #define MASK_IMPCAL_IMPDRVP 0x00000f00
    #define POS_IMPCAL_IMPDRVN 12
    #define MASK_IMPCAL_IMPDRVN 0x0000f000
#define DRAMC_REG_DMMonitor 0x1d8
#define DRAMC_REG_DRAMC_PD_CTRL 0x1dc
    #define MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK 0x00ff0000
    #define POS_DRAMC_PD_CTRL_REFCNT_FR_CLK 16
    #define POS_DRAMC_PD_CTRL_MIOCKCTRLOFF 26
#define DRAMC_REG_LPDDR2 0x1e0
    #define POS_LPDDR2_ADRDECEN 31
    #define POS_LPDDR2_SELO1ASO 30
#define DRAMC_REG_SPCMD 0x1e4
    #define POS_SPCMD_MRWEN 0
    #define POS_SPCMD_DQSGCNTEN 8
    #define POS_SPCMD_DQSGCNTRST 9
#define DRAMC_REG_ACTIM1 0x1e8
    #define MASK_ACTIM1_REFRCNT 0x00ff0000
    #define POS_ACTIM1_REFRCNT 16
#define DRAMC_REG_PERFCTL0 0x1ec
#define DRAMC_REG_RRRATE_CTL 0x1f4
    #define MASK_RRRATE_CTL_BIT0_SEL 0x0000001f
    #define MASK_RRRATE_CTL_BIT1_SEL 0x00001f00
    #define MASK_RRRATE_CTL_BIT2_SEL 0x001f0000
    #define MASK_RRRATE_CTL_BIT3_SEL 0x1f000000
    #define POS_RRRATE_CTL_BIT0_SEL 0
    #define POS_RRRATE_CTL_BIT1_SEL 8
    #define POS_RRRATE_CTL_BIT2_SEL 16
    #define POS_RRRATE_CTL_BIT3_SEL 24
#define DRAMC_REG_LPDDR2_4 0x1f8
#define DRAMC_REG_MRR_CTL 0x1fc
    #define MASK_MRR_CTL_BIT4_SEL 0x0000001f
    #define MASK_MRR_CTL_BIT5_SEL 0x00001f00
    #define MASK_MRR_CTL_BIT6_SEL 0x001f0000
    #define MASK_MRR_CTL_BIT7_SEL 0x1f000000
    #define POS_MRR_CTL_BIT4_SEL 0
    #define POS_MRR_CTL_BIT5_SEL 8
    #define POS_MRR_CTL_BIT6_SEL 16
    #define POS_MRR_CTL_BIT7_SEL 24
#define DRAMC_REG_DQODLY1 0x200
#define DRAMC_REG_DQODLY2 0x204
#define DRAMC_REG_DQODLY3 0x208
#define DRAMC_REG_DQODLY4 0x20c
#define DRAMC_REG_DQIDLY1 0x210
#define DRAMC_REG_DQIDLY2 0x214
#define DRAMC_REG_DQIDLY3 0x218
#define DRAMC_REG_DQIDLY4 0x21c
#define DRAMC_REG_DQIDLY5 0x220
#define DRAMC_REG_DQIDLY6 0x224
#define DRAMC_REG_DQIDLY7 0x228
#define DRAMC_REG_DQIDLY8 0x22c
#define DRAMC_REG_DMACON 0x234
    #define POS_DMACON_DMAEN 0
    #define POS_DMACON_DMAPUREWRITEEN 1
    #define POS_DMACON_DMAPUREREADEN 2
    #define POS_DMACON_DMABURSTLEN 4
    #define MASK_DMACON_DMABURSTLEN 0x00000070
#define DRAMC_REG_DMASRCADDR 0x238
#define DRAMC_REG_DMADSTADDR 0x23c
#define DRAMC_REG_DMATRANSLEN 0x240
    #define POS_DMATRANSLEN_DMATRANSLEN 0
    #define MASK_DMATRANSLEN_DMATRANSLEN 0x000fffff
#define DRAMC_REG_DMAPAT 0x244
#define DRAMC_REG_DMAMON 0x254
    #define POS_DMAMON_DMASTATUS 0
#define DRAMC_REG_DMACMPERR 0x258
#define DRAMC_REG_R2R_page_hit_counter 0x280
#define DRAMC_REG_STBENERR_R 0x320
#define DRAMC_REG_STBENERR_F 0x324
#define DRAMC_REG_TOGGLE_CNT 0x32c
#define DRAMC_REG_DQS_ERR_CNT 0x330
#define DRAMC_REG_DQ_ERR_CNT 0x334
#define DRAMC_REG_SELPH 0x344
#define DRAMC_REG_DCBLN 0x348
    #define POS_DCBLN_RSTBCNT_LATCH_EN 11
    #define POS_DCBLN_RX_MIOCK_JIT_EN 2
    #define POS_DCBLN_RX_EYE_SCAN_EN 1
    #define POS_DCBLN_REG_SW_RST 0
#define DRAMC_REG_TOGGLE_CNT_2 0x360
#define DRAMC_REG_DQS_ERR_CNT_2 0x364
#define DRAMC_REG_DQ_ERR_CNT_2 0x368
#define DRAMC_REG_CMP_ERR 0x370
#define DRAMC_REG_DQSDLY0 0x3a0
#define DRAMC_REG_SPCMDRESP 0x3b8
    #define POS_SPCMDRESP_SREF_STATE 16
#define DRAMC_REG_DQSGNWCNT0 0x3c0
#define DRAMC_REG_DQSGNWCNT1 0x3c4
#define DRAMC_REG_DLLSTATUS0 0x3dc
    #define POS_DLLSTATUS0_CMPOT 31
    #define POS_DLLSTATUS0_CMPOUTN 30
#define DRAMC_REG_TESTRPT 0x3fc
    #define POS_TESTRPT_DM_CMP_CPT 10
    #define POS_TESTRPT_DM_CMP_ERR 14
#define DRAMC_REG_SELPH1 0x400
#define DRAMC_REG_SELPH2 0x404
    #define MASK_SELPH2_TXDLY_DQSGATE 0x00007000
    #define POS_SELPH2_TXDLY_DQSGATE 12
    #define MASK_SELPH2_TXDLY_DQSGATE_P1 0x00700000
    #define POS_SELPH2_TXDLY_DQSGATE_P1 20
#define DRAMC_REG_SELPH3 0x408
#define DRAMC_REG_SELPH4 0x40C    
#define DRAMC_REG_SELPH5 0x410
    #define MASK_SELPH5_dly_DQSGATE 0x00c00000
    #define POS_SELPH5_dly_DQSGATE 22
    #define MASK_SELPH5_dly_DQSGATE_P1 0x03000000
    #define POS_SELPH5_dly_DQSGATE_P1 24
#define DRAMC_REG_SELPH6 0x414
#define DRAMC_REG_SELPH6_1 0x418
    #define MASK_SELPH6_1_TXDLY_R1DQSGATE 0x00000070
    #define POS_SELPH6_1_TXDLY_R1DQSGATE 4
    #define MASK_SELPH6_1_TXDLY_R1DQSGATE_P1 0x00000700
    #define POS_SELPH6_1_TXDLY_R1DQSGATE_P1 8
    #define MASK_SELPH6_1_dly_R1DQSGATE 0x00000003
    #define POS_SELPH6_1_dly_R1DQSGATE 0
    #define MASK_SELPH6_1_dly_R1DQSGATE_P1 0x0000000c
    #define POS_SELPH6_1_dly_R1DQSGATE_P1 2
#define DRAMC_REG_SELPH7 0x41c
#define DRAMC_REG_SELPH8 0x420
#define DRAMC_REG_SELPH9 0x424
#define DRAMC_REG_SELPH10 0x428
#define DRAMC_REG_SELPH11 0x42c
#define DRAMC_REG_SELPH12 0x430
#define DRAMC_REG_SELPH13 0x434
#define DRAMC_REG_SELPH14 0x438
#define DRAMC_REG_SELPH15 0x43c
#define DRAMC_REG_SELPH16 0x440
#define DRAMC_REG_COM0_DLY_SEL_DGTED 0x478
#define DRAMC_REG_COM0_DLY_SEL_DQS0 0x47C
#define DRAMC_REG_COM0_DLY_SEL_DQS1 0x480
#define DRAMC_REG_COM0_DLY_SEL_DQM0 0x484
#define DRAMC_REG_COM0_DLY_SEL_DQ2 0x494
#define DRAMC_REG_COM0_DLY_SEL_DQ3 0x498
#define DRAMC_REG_COM0_DLY_SEL_DQ4 0x49c
#define DRAMC_REG_COM0_DLY_SEL_DQ5 0x4a0
#define DRAMC_REG_COM0_DLY_SEL_DQ6 0x4a4
#define DRAMC_REG_COM0_DLY_SEL_DQ7 0x4a8
#define DRAMC_REG_COM1_DLY_SEL_DGTED 0x4CC
#define DRAMC_REG_COM1_DLY_SEL_DQS0 0x4D0
#define DRAMC_REG_COM1_DLY_SEL_DQS1 0x4D4
#define DRAMC_REG_COM1_DLY_SEL_DQM0 0x4D8
#define DRAMC_REG_COM1_DLY_SEL_DQ0 0x4e0
#define DRAMC_REG_COM1_DLY_SEL_DQ2 0x4e8
#define DRAMC_REG_COM1_DLY_SEL_DQ3 0x4ec
#define DRAMC_REG_COM1_DLY_SEL_DQ4 0x4f0
#define DRAMC_REG_COM1_DLY_SEL_DQ5 0x4f4
#define DRAMC_REG_COM1_DLY_SEL_DQ6 0x4f8
#define DRAMC_REG_COM1_DLY_SEL_DQ7 0x4fc
#define DRAMC_REG_PH_EN 0x520
#define DRAMC_REG_DLY_SEL_MCK_ANA 0x524
#define DRAMC_REG_DLY_SEL_CLKGEN 0x528
#define DRAMC_REG_DLY_SEL_CLKGEN2 0x52C

// modify if end register address change
#define A60808_REG_SIZE 0x400

#endif // _MT5133_REGISTER_H_
