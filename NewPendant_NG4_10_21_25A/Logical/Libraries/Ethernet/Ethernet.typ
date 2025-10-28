(********************************************************************
 * COPYRIGHT (C) BERNECKER + RAINER, AUSTRIA, A-5142 EGGELSBERG
 ********************************************************************
 * Library: Ethernet
 * File: Ethernet.typ
 * Created: 11.11.2003
 ********************************************************************
 * Global data types of library Ethernet
 ********************************************************************)
TYPE
	ETHattachment_typ : STRUCT
		type	: USINT ;
		len	: UDINT ;
		pName	: UDINT ;
		pData	: UDINT ;
	END_STRUCT;
	ETHstatSG3_typ : STRUCT
		l_int	: UDINT ;
		l_rint	: UDINT ;
		l_tint	: UDINT ;
		l_babl	: UINT ;
		l_cerr	: UINT ;
		l_miss	: UINT ;
		l_memr	: UINT ;
		r_fram	: UINT ;
		r_oflo	: UINT ;
		r_crc	: UINT ;
		r_buff	: UINT ;
		t_buff	: UINT ;
		t_uflo	: UINT ;
		t_lcol	: UINT ;
		t_lcar	: UINT ;
		t_rtry	: UINT ;
		ni_anno	: UINT ;
		ni_init	: UINT ;
		ni_send	: UINT ;
		ni_getp	: UINT ;
		ni_retp	: UINT ;
		ni_bcst	: UINT ;
		ni_poll	: UINT ;
		irq_mean	: UINT ;
		irq_min	: UINT ;
		irq_max	: UINT ;
		irq_t1	: UINT ;
		irq_timena	: UINT ;
		errgtnobuf	: UINT ;
		err_1_1	: UINT ;
		err_1_2	: UINT ;
		err_1_3	: UINT ;
		errxmtnull	: UINT ;
		errsndown	: UINT ;
		errsndsize	: UINT ;
		lancerrnobuf	: UINT ;
		lanceinit	: UINT ;
		terr	: UINT ;
		ni_gtbuf	: UINT ;
		brcast_cnt	: UINT ;
		brcastmax_cnt	: UINT ;
		brcast_stop	: UINT ;
		brcast_tick	: UINT ;
	END_STRUCT;
	ETHstatSG4_typ : STRUCT
		p_recv	: UDINT ;
		e_recv	: UDINT ;
		p_send	: UDINT ;
		e_send	: UDINT ;
		collisions	: UDINT ;
		b_recv	: UDINT ;
		b_send	: UDINT ;
		p_recv_mc	: UDINT ;
		p_send_mc	: UDINT ;
		iqdrops	: UDINT ;
		noproto	: UDINT ;
		lastchange	: UDINT ;
	END_STRUCT;
	TCPQ_dcmd_typ : STRUCT
		response	: UINT ;
		error	: UDINT ;
		buffer	: UDINT ;
		buflng	: UDINT ;
		ipaddr	: UDINT ;
		socket	: UDINT ;
		port	: UINT ;
		wreserve	: UINT ;
		reserve	: ARRAY[0..7] OF UDINT ;
	END_STRUCT;
END_TYPE