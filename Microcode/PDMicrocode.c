//RSP microcode

#include "../Shared/rsp_prefix.h"
#include "../Shared/rsp_macros.h"
#include "Microcode.h"
#include "TestMacros.h"

void _PDData(_u32 StartAddress);


void _PDBootCode(_u32 StartAddress)
{
	static _u32	JumpTable[(0x1000 >> 2)];
	static _u32	BuildJumpTable = 0;

	if (!BuildJumpTable)
	{
		BuildJumpTable = 1;
		goto _BuildJumpTable;
	}

	_Label:
	StartAddress = JumpTable[(StartAddress & 0x0FFF) >> 2];
	__asm mov eax, StartAddress
	__asm jmp eax


_04001000:	// 4001000: <09000419>	J         04001064
	state.r[AT] = 0 + 0x0FC0;
	goto _04001064;


_04001004:	// 4001004: <20010fc0>	ADDI      AT = R0 + 0fc0
	state.r[AT] = 0 + 0x0FC0;


_04001008:	// 4001008: <8c220010>	LW        V0 = [AT+0010]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000010) & 0xfff);


_0400100C:	// 400100c: <20030f7f>	ADDI      V1 = R0 + 0f7f
	state.r[V1] = 0 + 0x0F7F;


_04001010:	// 4001010: <20071080>	ADDI      A3 = R0 + 1080
	state.r[A3] = 0 + 0x1080;


_04001014:	// 4001014: <40870000>	MTC0      SP memory address = A3
	DMEM_Address = state.r[A3];


_04001018:	// 4001018: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400101C:	// 400101c: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001020;
	_PDData(0x04001020);
	return;


_04001020:	// 4001020: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001024:	// 4001024: <1480fffe>	BNE       (R0!=A0) --> 1020
	sp_reg_pc = 0x04001024;
	if (state.r[A0] != 0)
	{
		goto _04001020;
	}


_04001028:	// 4001028: <00000000>	NOP       


_0400102C:	// 400102c: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x0400102C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001030:	// 4001030: <00000000>	NOP       


_04001034:	// 4001034: <00e00008>	JR        A3
	{
		_u32	temp = ((state.r[A3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001038:	// 4001038: <40803800>	MTC0      SP semaphore = R0


_0400103C:	// 400103c: <40082000>	MFC0      A0 = DP CMD DMA start
	state.r[T0] = 0;


_04001040:	// 4001040: <31080080>	ANDI      T0 = T0 & 0080
	state.r[T0] = (state.r[T0] & 0x00000080);


_04001044:	// 4001044: <15000002>	BNE       (R0!=T0) --> 1050
	sp_reg_pc = 0x04001044;
	if (state.r[T0] != 0)
	{
		goto _04001050;
	}


_04001048:	// 4001048: <00000000>	NOP       


_0400104C:	// 400104c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001050:	// 4001050: <40803800>	MTC0      SP semaphore = R0


_04001054:	// 4001054: <34085200>	ORI       T0 = R0 | 0x5200
	state.r[T0] = (0 | 0x00005200);


_04001058:	// 4001058: <40882000>	MTC0      SP status = T0


_0400105C:	// 400105c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001060:	// 4001060: <00000000>	NOP       


_04001064:	// 4001064: <8c220004>	LW        V0 = [AT+0004]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001068:	// 4001068: <30420002>	ANDI      V0 = V0 & 0002
	state.r[V0] = (state.r[V0] & 0x00000002);


_0400106C:	// 400106c: <10400007>	BEQ       (R0==V0) --> 108c
	sp_reg_pc = 0x0400106C;
	if (state.r[V0] == 0)
	{
		goto _0400108C;
	}


_04001070:	// 4001070: <00000000>	NOP       


_04001074:	// 4001074: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x04001074;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001078:	// 4001078: <00000000>	NOP       


_0400107C:	// 400107c: <40025800>	MFC0      T3 = SP read DMA length
	state.r[V0] = 0;


_04001080:	// 4001080: <30420100>	ANDI      V0 = V0 & 0100
	state.r[V0] = (state.r[V0] & 0x00000100);


_04001084:	// 4001084: <1c40ffed>	BGTZ      (R0>0) --> 103c
	sp_reg_pc = 0x04001084;
	if((_s32)state.r[V0] > 0)
	{
		goto _0400103C;
	}


_04001088:	// 4001088: <00000000>	NOP       


_0400108C:	// 400108c: <8c220018>	LW        V0 = [AT+0018]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000018) & 0xfff);


_04001090:	// 4001090: <8c23001c>	LW        V1 = [AT+001c]
	state.r[V1] = (_s32)Load32_DMEM((state.r[AT] + 0x0000001C) & 0xfff);


_04001094:	// 4001094: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001098:	// 4001098: <401e2800>	MFC0      A1 = unknown
	state.r[S8] = 0;


_0400109C:	// 400109c: <17c0fffe>	BNE       (R0!=S8) --> 1098
	sp_reg_pc = 0x0400109C;
	if (state.r[S8] != 0)
	{
		goto _04001098;
	}


_040010A0:	// 40010a0: <00000000>	NOP       


_040010A4:	// 40010a4: <40800000>	MTC0      SP memory address = R0
	DMEM_Address = 0;


_040010A8:	// 40010a8: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040010AC:	// 40010ac: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040010AC;
	DMARead(state.r[V1]);


_040010B0:	// 40010b0: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_040010B4:	// 40010b4: <1480fffe>	BNE       (R0!=A0) --> 10b0
	sp_reg_pc = 0x040010B4;
	if (state.r[A0] != 0)
	{
		goto _040010B0;
	}


_040010B8:	// 40010b8: <00000000>	NOP       


_040010BC:	// 40010bc: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x040010BC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_040010C0:	// 40010c0: <00000000>	NOP       


_040010C4:	// 40010c4: <09000402>	J         04001008
	goto _04001008;


_040010C8:	// 40010c8: <00000000>	NOP       

_040010CC:
_040010D0:
_040010D4:
_040010D8:
_040010DC:
_040010E0:
_040010E4:
_040010E8:
_040010EC:
_040010F0:
_040010F4:
_040010F8:
_040010FC:
_04001100:
_04001104:
_04001108:
_0400110C:
_04001110:
_04001114:
_04001118:
_0400111C:
_04001120:
_04001124:
_04001128:
_0400112C:
_04001130:
_04001134:
_04001138:
_0400113C:
_04001140:
_04001144:
_04001148:
_0400114C:
_04001150:
_04001154:
_04001158:
_0400115C:
_04001160:
_04001164:
_04001168:
_0400116C:
_04001170:
_04001174:
_04001178:
_0400117C:
_04001180:
_04001184:
_04001188:
_0400118C:
_04001190:
_04001194:
_04001198:
_0400119C:
_040011A0:
_040011A4:
_040011A8:
_040011AC:
_040011B0:
_040011B4:
_040011B8:
_040011BC:
_040011C0:
_040011C4:
_040011C8:
_040011CC:
_040011D0:
_040011D4:
_040011D8:
_040011DC:
_040011E0:
_040011E4:
_040011E8:
_040011EC:
_040011F0:
_040011F4:
_040011F8:
_040011FC:
_04001200:
_04001204:
_04001208:
_0400120C:
_04001210:
_04001214:
_04001218:
_0400121C:
_04001220:
_04001224:
_04001228:
_0400122C:
_04001230:
_04001234:
_04001238:
_0400123C:
_04001240:
_04001244:
_04001248:
_0400124C:
_04001250:
_04001254:
_04001258:
_0400125C:
_04001260:
_04001264:
_04001268:
_0400126C:
_04001270:
_04001274:
_04001278:
_0400127C:
_04001280:
_04001284:
_04001288:
_0400128C:
_04001290:
_04001294:
_04001298:
_0400129C:
_040012A0:
_040012A4:
_040012A8:
_040012AC:
_040012B0:
_040012B4:
_040012B8:
_040012BC:
_040012C0:
_040012C4:
_040012C8:
_040012CC:
_040012D0:
_040012D4:
_040012D8:
_040012DC:
_040012E0:
_040012E4:
_040012E8:
_040012EC:
_040012F0:
_040012F4:
_040012F8:
_040012FC:
_04001300:
_04001304:
_04001308:
_0400130C:
_04001310:
_04001314:
_04001318:
_0400131C:
_04001320:
_04001324:
_04001328:
_0400132C:
_04001330:
_04001334:
_04001338:
_0400133C:
_04001340:
_04001344:
_04001348:
_0400134C:
_04001350:
_04001354:
_04001358:
_0400135C:
_04001360:
_04001364:
_04001368:
_0400136C:
_04001370:
_04001374:
_04001378:
_0400137C:
_04001380:
_04001384:
_04001388:
_0400138C:
_04001390:
_04001394:
_04001398:
_0400139C:
_040013A0:
_040013A4:
_040013A8:
_040013AC:
_040013B0:
_040013B4:
_040013B8:
_040013BC:
_040013C0:
_040013C4:
_040013C8:
_040013CC:
_040013D0:
_040013D4:
_040013D8:
_040013DC:
_040013E0:
_040013E4:
_040013E8:
_040013EC:
_040013F0:
_040013F4:
_040013F8:
_040013FC:
_04001400:
_04001404:
_04001408:
_0400140C:
_04001410:
_04001414:
_04001418:
_0400141C:
_04001420:
_04001424:
_04001428:
_0400142C:
_04001430:
_04001434:
_04001438:
_0400143C:
_04001440:
_04001444:
_04001448:
_0400144C:
_04001450:
_04001454:
_04001458:
_0400145C:
_04001460:
_04001464:
_04001468:
_0400146C:
_04001470:
_04001474:
_04001478:
_0400147C:
_04001480:
_04001484:
_04001488:
_0400148C:
_04001490:
_04001494:
_04001498:
_0400149C:
_040014A0:
_040014A4:
_040014A8:
_040014AC:
_040014B0:
_040014B4:
_040014B8:
_040014BC:
_040014C0:
_040014C4:
_040014C8:
_040014CC:
_040014D0:
_040014D4:
_040014D8:
_040014DC:
_040014E0:
_040014E4:
_040014E8:
_040014EC:
_040014F0:
_040014F4:
_040014F8:
_040014FC:
_04001500:
_04001504:
_04001508:
_0400150C:
_04001510:
_04001514:
_04001518:
_0400151C:
_04001520:
_04001524:
_04001528:
_0400152C:
_04001530:
_04001534:
_04001538:
_0400153C:
_04001540:
_04001544:
_04001548:
_0400154C:
_04001550:
_04001554:
_04001558:
_0400155C:
_04001560:
_04001564:
_04001568:
_0400156C:
_04001570:
_04001574:
_04001578:
_0400157C:
_04001580:
_04001584:
_04001588:
_0400158C:
_04001590:
_04001594:
_04001598:
_0400159C:
_040015A0:
_040015A4:
_040015A8:
_040015AC:
_040015B0:
_040015B4:
_040015B8:
_040015BC:
_040015C0:
_040015C4:
_040015C8:
_040015CC:
_040015D0:
_040015D4:
_040015D8:
_040015DC:
_040015E0:
_040015E4:
_040015E8:
_040015EC:
_040015F0:
_040015F4:
_040015F8:
_040015FC:
_04001600:
_04001604:
_04001608:
_0400160C:
_04001610:
_04001614:
_04001618:
_0400161C:
_04001620:
_04001624:
_04001628:
_0400162C:
_04001630:
_04001634:
_04001638:
_0400163C:
_04001640:
_04001644:
_04001648:
_0400164C:
_04001650:
_04001654:
_04001658:
_0400165C:
_04001660:
_04001664:
_04001668:
_0400166C:
_04001670:
_04001674:
_04001678:
_0400167C:
_04001680:
_04001684:
_04001688:
_0400168C:
_04001690:
_04001694:
_04001698:
_0400169C:
_040016A0:
_040016A4:
_040016A8:
_040016AC:
_040016B0:
_040016B4:
_040016B8:
_040016BC:
_040016C0:
_040016C4:
_040016C8:
_040016CC:
_040016D0:
_040016D4:
_040016D8:
_040016DC:
_040016E0:
_040016E4:
_040016E8:
_040016EC:
_040016F0:
_040016F4:
_040016F8:
_040016FC:
_04001700:
_04001704:
_04001708:
_0400170C:
_04001710:
_04001714:
_04001718:
_0400171C:
_04001720:
_04001724:
_04001728:
_0400172C:
_04001730:
_04001734:
_04001738:
_0400173C:
_04001740:
_04001744:
_04001748:
_0400174C:
_04001750:
_04001754:
_04001758:
_0400175C:
_04001760:
_04001764:
_04001768:
_0400176C:
_04001770:
_04001774:
_04001778:
_0400177C:
_04001780:
_04001784:
_04001788:
_0400178C:
_04001790:
_04001794:
_04001798:
_0400179C:
_040017A0:
_040017A4:
_040017A8:
_040017AC:
_040017B0:
_040017B4:
_040017B8:
_040017BC:
_040017C0:
_040017C4:
_040017C8:
_040017CC:
_040017D0:
_040017D4:
_040017D8:
_040017DC:
_040017E0:
_040017E4:
_040017E8:
_040017EC:
_040017F0:
_040017F4:
_040017F8:
_040017FC:
_04001800:
_04001804:
_04001808:
_0400180C:
_04001810:
_04001814:
_04001818:
_0400181C:
_04001820:
_04001824:
_04001828:
_0400182C:
_04001830:
_04001834:
_04001838:
_0400183C:
_04001840:
_04001844:
_04001848:
_0400184C:
_04001850:
_04001854:
_04001858:
_0400185C:
_04001860:
_04001864:
_04001868:
_0400186C:
_04001870:
_04001874:
_04001878:
_0400187C:
_04001880:
_04001884:
_04001888:
_0400188C:
_04001890:
_04001894:
_04001898:
_0400189C:
_040018A0:
_040018A4:
_040018A8:
_040018AC:
_040018B0:
_040018B4:
_040018B8:
_040018BC:
_040018C0:
_040018C4:
_040018C8:
_040018CC:
_040018D0:
_040018D4:
_040018D8:
_040018DC:
_040018E0:
_040018E4:
_040018E8:
_040018EC:
_040018F0:
_040018F4:
_040018F8:
_040018FC:
_04001900:
_04001904:
_04001908:
_0400190C:
_04001910:
_04001914:
_04001918:
_0400191C:
_04001920:
_04001924:
_04001928:
_0400192C:
_04001930:
_04001934:
_04001938:
_0400193C:
_04001940:
_04001944:
_04001948:
_0400194C:
_04001950:
_04001954:
_04001958:
_0400195C:
_04001960:
_04001964:
_04001968:
_0400196C:
_04001970:
_04001974:
_04001978:
_0400197C:
_04001980:
_04001984:
_04001988:
_0400198C:
_04001990:
_04001994:
_04001998:
_0400199C:
_040019A0:
_040019A4:
_040019A8:
_040019AC:
_040019B0:
_040019B4:
_040019B8:
_040019BC:
_040019C0:
_040019C4:
_040019C8:
_040019CC:
_040019D0:
_040019D4:
_040019D8:
_040019DC:
_040019E0:
_040019E4:
_040019E8:
_040019EC:
_040019F0:
_040019F4:
_040019F8:
_040019FC:
_04001A00:
_04001A04:
_04001A08:
_04001A0C:
_04001A10:
_04001A14:
_04001A18:
_04001A1C:
_04001A20:
_04001A24:
_04001A28:
_04001A2C:
_04001A30:
_04001A34:
_04001A38:
_04001A3C:
_04001A40:
_04001A44:
_04001A48:
_04001A4C:
_04001A50:
_04001A54:
_04001A58:
_04001A5C:
_04001A60:
_04001A64:
_04001A68:
_04001A6C:
_04001A70:
_04001A74:
_04001A78:
_04001A7C:
_04001A80:
_04001A84:
_04001A88:
_04001A8C:
_04001A90:
_04001A94:
_04001A98:
_04001A9C:
_04001AA0:
_04001AA4:
_04001AA8:
_04001AAC:
_04001AB0:
_04001AB4:
_04001AB8:
_04001ABC:
_04001AC0:
_04001AC4:
_04001AC8:
_04001ACC:
_04001AD0:
_04001AD4:
_04001AD8:
_04001ADC:
_04001AE0:
_04001AE4:
_04001AE8:
_04001AEC:
_04001AF0:
_04001AF4:
_04001AF8:
_04001AFC:
_04001B00:
_04001B04:
_04001B08:
_04001B0C:
_04001B10:
_04001B14:
_04001B18:
_04001B1C:
_04001B20:
_04001B24:
_04001B28:
_04001B2C:
_04001B30:
_04001B34:
_04001B38:
_04001B3C:
_04001B40:
_04001B44:
_04001B48:
_04001B4C:
_04001B50:
_04001B54:
_04001B58:
_04001B5C:
_04001B60:
_04001B64:
_04001B68:
_04001B6C:
_04001B70:
_04001B74:
_04001B78:
_04001B7C:
_04001B80:
_04001B84:
_04001B88:
_04001B8C:
_04001B90:
_04001B94:
_04001B98:
_04001B9C:
_04001BA0:
_04001BA4:
_04001BA8:
_04001BAC:
_04001BB0:
_04001BB4:
_04001BB8:
_04001BBC:
_04001BC0:
_04001BC4:
_04001BC8:
_04001BCC:
_04001BD0:
_04001BD4:
_04001BD8:
_04001BDC:
_04001BE0:
_04001BE4:
_04001BE8:
_04001BEC:
_04001BF0:
_04001BF4:
_04001BF8:
_04001BFC:
_04001C00:
_04001C04:
_04001C08:
_04001C0C:
_04001C10:
_04001C14:
_04001C18:
_04001C1C:
_04001C20:
_04001C24:
_04001C28:
_04001C2C:
_04001C30:
_04001C34:
_04001C38:
_04001C3C:
_04001C40:
_04001C44:
_04001C48:
_04001C4C:
_04001C50:
_04001C54:
_04001C58:
_04001C5C:
_04001C60:
_04001C64:
_04001C68:
_04001C6C:
_04001C70:
_04001C74:
_04001C78:
_04001C7C:
_04001C80:
_04001C84:
_04001C88:
_04001C8C:
_04001C90:
_04001C94:
_04001C98:
_04001C9C:
_04001CA0:
_04001CA4:
_04001CA8:
_04001CAC:
_04001CB0:
_04001CB4:
_04001CB8:
_04001CBC:
_04001CC0:
_04001CC4:
_04001CC8:
_04001CCC:
_04001CD0:
_04001CD4:
_04001CD8:
_04001CDC:
_04001CE0:
_04001CE4:
_04001CE8:
_04001CEC:
_04001CF0:
_04001CF4:
_04001CF8:
_04001CFC:
_04001D00:
_04001D04:
_04001D08:
_04001D0C:
_04001D10:
_04001D14:
_04001D18:
_04001D1C:
_04001D20:
_04001D24:
_04001D28:
_04001D2C:
_04001D30:
_04001D34:
_04001D38:
_04001D3C:
_04001D40:
_04001D44:
_04001D48:
_04001D4C:
_04001D50:
_04001D54:
_04001D58:
_04001D5C:
_04001D60:
_04001D64:
_04001D68:
_04001D6C:
_04001D70:
_04001D74:
_04001D78:
_04001D7C:
_04001D80:
_04001D84:
_04001D88:
_04001D8C:
_04001D90:
_04001D94:
_04001D98:
_04001D9C:
_04001DA0:
_04001DA4:
_04001DA8:
_04001DAC:
_04001DB0:
_04001DB4:
_04001DB8:
_04001DBC:
_04001DC0:
_04001DC4:
_04001DC8:
_04001DCC:
_04001DD0:
_04001DD4:
_04001DD8:
_04001DDC:
_04001DE0:
_04001DE4:
_04001DE8:
_04001DEC:
_04001DF0:
_04001DF4:
_04001DF8:
_04001DFC:
_04001E00:
_04001E04:
_04001E08:
_04001E0C:
_04001E10:
_04001E14:
_04001E18:
_04001E1C:
_04001E20:
_04001E24:
_04001E28:
_04001E2C:
_04001E30:
_04001E34:
_04001E38:
_04001E3C:
_04001E40:
_04001E44:
_04001E48:
_04001E4C:
_04001E50:
_04001E54:
_04001E58:
_04001E5C:
_04001E60:
_04001E64:
_04001E68:
_04001E6C:
_04001E70:
_04001E74:
_04001E78:
_04001E7C:
_04001E80:
_04001E84:
_04001E88:
_04001E8C:
_04001E90:
_04001E94:
_04001E98:
_04001E9C:
_04001EA0:
_04001EA4:
_04001EA8:
_04001EAC:
_04001EB0:
_04001EB4:
_04001EB8:
_04001EBC:
_04001EC0:
_04001EC4:
_04001EC8:
_04001ECC:
_04001ED0:
_04001ED4:
_04001ED8:
_04001EDC:
_04001EE0:
_04001EE4:
_04001EE8:
_04001EEC:
_04001EF0:
_04001EF4:
_04001EF8:
_04001EFC:
_04001F00:
_04001F04:
_04001F08:
_04001F0C:
_04001F10:
_04001F14:
_04001F18:
_04001F1C:
_04001F20:
_04001F24:
_04001F28:
_04001F2C:
_04001F30:
_04001F34:
_04001F38:
_04001F3C:
_04001F40:
_04001F44:
_04001F48:
_04001F4C:
_04001F50:
_04001F54:
_04001F58:
_04001F5C:
_04001F60:
_04001F64:
_04001F68:
_04001F6C:
_04001F70:
_04001F74:
_04001F78:
_04001F7C:
_04001F80:
_04001F84:
_04001F88:
_04001F8C:
_04001F90:
_04001F94:
_04001F98:
_04001F9C:
_04001FA0:
_04001FA4:
_04001FA8:
_04001FAC:
_04001FB0:
_04001FB4:
_04001FB8:
_04001FBC:
_04001FC0:
_04001FC4:
_04001FC8:
_04001FCC:
_04001FD0:
_04001FD4:
_04001FD8:
_04001FDC:
_04001FE0:
_04001FE4:
_04001FE8:
_04001FEC:
_04001FF0:
_04001FF4:
_04001FF8:
_04001FFC:
	state.halt = 1;
	return;

	_BuildJumpTable:
	#include "jumptable.h"
	goto _Label;
}

void _PDData(_u32 StartAddress)
{
	static _u32	JumpTable[(0x1000 >> 2)];
	static _u32	BuildJumpTable = 0;

	if (!BuildJumpTable)
	{
		BuildJumpTable = 1;
		goto _BuildJumpTable;
	}

	_Label:
	StartAddress = JumpTable[(StartAddress & 0x0FFF) >> 2];
	__asm mov eax, StartAddress
	__asm jmp eax


_04001000:	// 4001000: <09000419>	J         04001064
	state.r[AT] = 0 + 0x0FC0;
	goto _04001064;


_04001004:	// 4001004: <20010fc0>	ADDI      AT = R0 + 0fc0
	state.r[AT] = 0 + 0x0FC0;


_04001008:	// 4001008: <8c220010>	LW        V0 = [AT+0010]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000010) & 0xfff);


_0400100C:	// 400100c: <20030f7f>	ADDI      V1 = R0 + 0f7f
	state.r[V1] = 0 + 0x0F7F;


_04001010:	// 4001010: <20071080>	ADDI      A3 = R0 + 1080
	state.r[A3] = 0 + 0x1080;


_04001014:	// 4001014: <40870000>	MTC0      SP memory address = A3
	DMEM_Address = state.r[A3];


_04001018:	// 4001018: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400101C:	// 400101c: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001020;
	_PDData(0x04001020);
	return;


_04001020:	// 4001020: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001024:	// 4001024: <1480fffe>	BNE       (R0!=A0) --> 1020
	sp_reg_pc = 0x04001024;
	if (state.r[A0] != 0)
	{
		goto _04001020;
	}


_04001028:	// 4001028: <00000000>	NOP       


_0400102C:	// 400102c: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x0400102C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001030:	// 4001030: <00000000>	NOP       


_04001034:	// 4001034: <00e00008>	JR        A3
	{
		_u32	temp = ((state.r[A3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001038:	// 4001038: <40803800>	MTC0      SP semaphore = R0


_0400103C:	// 400103c: <40082000>	MFC0      A0 = DP CMD DMA start
	state.r[T0] = 0;


_04001040:	// 4001040: <31080080>	ANDI      T0 = T0 & 0080
	state.r[T0] = (state.r[T0] & 0x00000080);


_04001044:	// 4001044: <15000002>	BNE       (R0!=T0) --> 1050
	sp_reg_pc = 0x04001044;
	if (state.r[T0] != 0)
	{
		goto _04001050;
	}


_04001048:	// 4001048: <00000000>	NOP       


_0400104C:	// 400104c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001050:	// 4001050: <40803800>	MTC0      SP semaphore = R0


_04001054:	// 4001054: <34085200>	ORI       T0 = R0 | 0x5200
	state.r[T0] = (0 | 0x00005200);


_04001058:	// 4001058: <40882000>	MTC0      SP status = T0


_0400105C:	// 400105c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001060:	// 4001060: <00000000>	NOP       


_04001064:	// 4001064: <8c220004>	LW        V0 = [AT+0004]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001068:	// 4001068: <30420002>	ANDI      V0 = V0 & 0002
	state.r[V0] = (state.r[V0] & 0x00000002);


_0400106C:	// 400106c: <10400007>	BEQ       (R0==V0) --> 108c
	sp_reg_pc = 0x0400106C;
	if (state.r[V0] == 0)
	{
		goto _0400108C;
	}


_04001070:	// 4001070: <00000000>	NOP       


_04001074:	// 4001074: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x04001074;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001078:	// 4001078: <00000000>	NOP       


_0400107C:	// 400107c: <40025800>	MFC0      T3 = SP read DMA length
	state.r[V0] = 0;


_04001080:	// 4001080: <40055800>	MFC0      T3 = SP DMA full
	state.r[A1] = 0;


_04001084:	// 4001084: <8c3c0030>	LW        GP = [AT+0030]
	state.r[GP] = (_s32)Load32_DMEM((state.r[AT] + 0x00000030) & 0xfff);


_04001088:	// 4001088: <8c3b0034>	LW        K1 = [AT+0034]
	state.r[K1] = (_s32)Load32_DMEM((state.r[AT] + 0x00000034) & 0xfff);


_0400108C:	// 400108c: <30a40001>	ANDI      A0 = A1 & 0001
	state.r[A0] = (state.r[A1] & 0x00000001);


_04001090:	// 4001090: <10800006>	BEQ       (R0==A0) --> 10ac
	sp_reg_pc = 0x04001090;
	if (state.r[A0] == 0)
	{
		state.r[A0] = (state.r[A1] & 0x00000100);
		goto _040010AC;
	}


_04001094:	// 4001094: <30a40100>	ANDI      A0 = A1 & 0100
	state.r[A0] = (state.r[A1] & 0x00000100);


_04001098:	// 4001098: <10800004>	BEQ       (R0==A0) --> 10ac
	sp_reg_pc = 0x04001098;
	if (state.r[A0] == 0)
	{
		state.r[A0] = 0;
		goto _040010AC;
	}


_0400109C:	// 400109c: <40045800>	MFC0      T3 = SP status
	state.r[A0] = 0;


_040010A0:	// 40010a0: <30840100>	ANDI      A0 = A0 & 0100
	state.r[A0] = (state.r[A0] & 0x00000100);


_040010A4:	// 40010a4: <1c80fffe>	BGTZ      (R0>0) --> 10a0
	sp_reg_pc = 0x040010A4;
	if((_s32)state.r[A0] > 0)
	{
		state.r[A0] = 0;
		goto _040010A0;
	}


_040010A8:	// 40010a8: <40045800>	MFC0      T3 = SP status
	state.r[A0] = 0;


_040010AC:	// 40010ac: <20180fa0>	ADDI      T8 = R0 + 0fa0
	state.r[T8] = 0 + 0x0FA0;


_040010B0:	// 40010b0: <8c250010>	LW        A1 = [AT+0010]
	state.r[A1] = (_s32)Load32_DMEM((state.r[AT] + 0x00000010) & 0xfff);


_040010B4:	// 40010b4: <8c040000>	LW        A0 = [R0+0000]
	state.r[A0] = (_s32)Load32_DMEM((0 + 0x00000000) & 0xfff);


_040010B8:	// 40010b8: <00852020>	ADD       A0 = A0+A1
	state.r[A0] = (state.r[A0] + state.r[A1]);


_040010BC:	// 40010bc: <ac040000>	SW        [R0+0000] = A0
	Save32_DMEM((_u32)state.r[A0], (0 + 0x00000000) & 0xfff);


_040010C0:	// 40010c0: <8c040008>	LW        A0 = [R0+0008]
	state.r[A0] = (_s32)Load32_DMEM((0 + 0x00000008) & 0xfff);


_040010C4:	// 40010c4: <00852020>	ADD       A0 = A0+A1
	state.r[A0] = (state.r[A0] + state.r[A1]);


_040010C8:	// 40010c8: <ac040008>	SW        [R0+0008] = A0
	Save32_DMEM((_u32)state.r[A0], (0 + 0x00000008) & 0xfff);


_040010CC:	// 40010cc: <8c250018>	LW        A1 = [AT+0018]
	state.r[A1] = (_s32)Load32_DMEM((state.r[AT] + 0x00000018) & 0xfff);


_040010D0:	// 40010d0: <ac050ff8>	SW        [R0+0ff8] = A1
	Save32_DMEM((_u32)state.r[A1], (0 + 0x00000FF8) & 0xfff);


_040010D4:	// 40010d4: <0d000454>	JAL	    04001150
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x040010D4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001150;


_040010D8:	// 40010d8: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_040010DC:	// 40010dc: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_040010E0:	// 40010e0: <1440ffff>	BNE       (R0!=V0) --> 10e0
	sp_reg_pc = 0x040010E0;
	if (state.r[V0] != 0)
	{
		state.r[V0] = 0;
		goto _040010E0;
	}


_040010E4:	// 40010e4: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_040010E8:	// 40010e8: <8fba0000>	LW        K0 = [SP+0000]
	// In function: Main_Parser_Tooie
	//Call the function: Main_Parser_Tooie
	Main_Parser_Tooie();
	goto _04001100;

_040010EC:	// 40010ec: <8fb90004>	LW        T9 = [SP+0004]
	ErrorToCallFunction(0x040010EC);

_040010F0:	// 40010f0: <239c0008>	ADDI      GP = GP + 0008
	ErrorToCallFunction(0x040010F0);

_040010F4:	// 40010f4: <001a0dc2>	SRL       AT = K0 >> 23
	ErrorToCallFunction(0x040010F4);

_040010F8:	// 40010f8: <302100fe>	ANDI      AT = AT & 00fe
	ErrorToCallFunction(0x040010F8);

_040010FC:	// 40010fc: <84210010>	LH        AT = [AT+0010]
	ErrorToCallFunction(0x040010FC);

_04001100:	// 4001100: <00200008>	JR        AT
	{
		_u32	temp = ((state.r[AT] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[K1] = state.r[K1] + 0xFFFFFFF8;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001104:	// 4001104: <237bfff8>	ADDI      K1 = K1 + fff8
	state.r[K1] = state.r[K1] + 0xFFFFFFF8;


_04001108:	// 4001108: <0000000d>	BREAK
	state.halt = 1;
	return;


_0400110C:	// 400110c: <1fc0fff6>	BGTZ      (R0>0) --> 10e8
	sp_reg_pc = 0x0400110C;
	if((_s32)state.r[S8] > 0)
	{
		state.r[SP] = state.r[SP] + 0x0008;
		goto _040010E8;
	}


_04001110:	// 4001110: <23bd0008>	ADDI      SP = SP + 0008
	state.r[SP] = state.r[SP] + 0x0008;


_04001114:	// 4001114: <1b600005>	BLEZ      (R0<=0) --> 112c
	sp_reg_pc = 0x04001114;
	if ((_s32)state.r[K1] <= 0)
	{
		state.r[AT] = (0 | 0x00004000);
		goto _0400112C;
	}


_04001118:	// 4001118: <34014000>	ORI       AT = R0 | 0x4000
	state.r[AT] = (0 | 0x00004000);


_0400111C:	// 400111c: <0d000454>	JAL	    04001150
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x0400111C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001150;


_04001120:	// 4001120: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_04001124:	// 4001124: <09000438>	J         040010e0
	state.r[V0] = 0;
	goto _040010E0;


_04001128:	// 4001128: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_0400112C:	// 400112c: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_04001130:	// 4001130: <1440fffe>	BNE       (R0!=V0) --> 112c
	sp_reg_pc = 0x04001130;
	if (state.r[V0] != 0)
	{
		goto _0400112C;
	}


_04001134:	// 4001134: <00000000>	NOP       


_04001138:	// 4001138: <40803800>	MTC0      SP semaphore = R0


_0400113C:	// 400113c: <40812000>	MTC0      SP status = AT


_04001140:	// 4001140: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001144:	// 4001144: <00000000>	NOP       


_04001148:	// 4001148: <1000ffff>	BEQ       (R0==R0) --> 1148
	sp_reg_pc = 0x04001148;
	if (0 == 0)
	{
		goto _04001148;
	}


_0400114C:	// 400114c: <00000000>	NOP       


_04001150:	// 4001150: <23e50000>	ADDI      A1 = RA + 0000
	state.r[A1] = state.r[RA] + 0x0000;


_04001154:	// 4001154: <23630000>	ADDI      V1 = K1 + 0000
	state.r[V1] = state.r[K1] + 0x0000;


_04001158:	// 4001158: <2064fec0>	ADDI      A0 = V1 + fec0
	state.r[A0] = state.r[V1] + 0xFFFFFEC0;


_0400115C:	// 400115c: <18800002>	BLEZ      (R0<=0) --> 1168
	sp_reg_pc = 0x0400115C;
	if ((_s32)state.r[A0] <= 0)
	{
		state.r[AT] = 0 + 0x02C0;
		goto _04001168;
	}


_04001160:	// 4001160: <200102c0>	ADDI      AT = R0 + 02c0
	state.r[AT] = 0 + 0x02C0;


_04001164:	// 4001164: <20030140>	ADDI      V1 = R0 + 0140
	state.r[V1] = 0 + 0x0140;


_04001168:	// 4001168: <207e0000>	ADDI      S8 = V1 + 0000
	state.r[S8] = state.r[V1] + 0x0000;


_0400116C:	// 400116c: <0d00045f>	JAL	    0400117c
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x0400116C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001170:	// 4001170: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001174:	// 4001174: <00a00008>	JR        A1
	{
		_u32	temp = ((state.r[A1] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[SP] = 0 + 0x02C0;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001178:	// 4001178: <201d02c0>	ADDI      SP = R0 + 02c0
	state.r[SP] = 0 + 0x02C0;


_0400117C:	// 400117c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001180:	// 4001180: <1480ffff>	BNE       (R0!=A0) --> 1180
	sp_reg_pc = 0x04001180;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001180;
	}


_04001184:	// 4001184: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001188:	// 4001188: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400118C:	// 400118c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001190:	// 4001190: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		sp_reg_pc = 0x04001194;
		DMARead(state.r[V1]);
		{
			extern int gSwitchToFunction;
			extern void _PD_MP3(_u32 StartAddress);
			
			if (( gSwitchToFunction >= 0)&&(ucodeInfos[gUcode].DMAMaps[gSwitchToFunction].dword2 == 0))
				MessageBox(0, "MP3", "", 0);

			
			if( gSwitchToFunction >= 0 && ucodeInfos[gUcode].DMAMaps[gSwitchToFunction].functionAddress != _PDData )
			{
				int functionToSwitchTo = gSwitchToFunction;
				gSwitchToFunction = -1;
				ucodeInfos[gUcode].DMAMaps[functionToSwitchTo].functionAddress(state.r[RA]);
				return;
			}
		}
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001194:	// 4001194: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001194;
	DMARead(state.r[V1]);
	{
		extern int gSwitchToFunction;
		extern void _PD_MP3(_u32 StartAddress);
		if( gSwitchToFunction >= 0 && ucodeInfos[gUcode].DMAMaps[gSwitchToFunction].functionAddress != _PDData )
		{
			int functionToSwitchTo = gSwitchToFunction;
			gSwitchToFunction = -1;
			ucodeInfos[gUcode].DMAMaps[functionToSwitchTo].functionAddress(state.r[RA]);
			return;
		}
	}


_04001198:	// 4001198: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_0400119C:	// 400119c: <1480ffff>	BNE       (R0!=A0) --> 119c
	sp_reg_pc = 0x0400119C;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _0400119C;
	}


_040011A0:	// 40011a0: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_040011A4:	// 40011a4: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040011A8:	// 40011a8: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040011AC:	// 40011ac: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		DMAWrite(state.r[V1]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040011B0:	// 40011b0: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_040011B4:	// 40011b4: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_040011B8:	// 40011b8: <1480fffe>	BNE       (R0!=A0) --> 11b4
	sp_reg_pc = 0x040011B8;
	if (state.r[A0] != 0)
	{
		goto _040011B4;
	}


_040011BC:	// 40011bc: <00000000>	NOP       


_040011C0:	// 40011c0: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040011C4:	// 40011c4: <00000000>	NOP       


_040011C8:	// 40011c8: <84030004>	LH        V1 = [R0+0004]
	state.r[V1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000004) & 0xfff);


_040011CC:	// 40011cc: <8c020000>	LW        V0 = [R0+0000]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000000) & 0xfff);


_040011D0:	// 40011d0: <8401000e>	LH        AT = [R0+000e]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);


_040011D4:	// 40011d4: <00411020>	ADD       V0 = V0+AT
	state.r[V0] = (state.r[V0] + state.r[AT]);


_040011D8:	// 40011d8: <00611822>	SUB       V1 = V1-AT
	state.r[V1] = (state.r[V1] - state.r[AT]);


_040011DC:	// 40011dc: <84010006>	LH        AT = [R0+0006]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x00000006) & 0xfff);


_040011E0:	// 40011e0: <00411022>	SUB       V0 = V0-AT
	state.r[V0] = (state.r[V0] - state.r[AT]);


_040011E4:	// 40011e4: <00611820>	ADD       V1 = V1+AT
	state.r[V1] = (state.r[V1] + state.r[AT]);


_040011E8:	// 40011e8: <0d00045f>	JAL	    0400117c
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);
	sp_reg_pc = 0x040011E8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_040011EC:	// 40011ec: <8401000e>	LH        AT = [R0+000e]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);


_040011F0:	// 40011f0: <8c020ff8>	LW        V0 = [R0+0ff8]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000FF8) & 0xfff);


_040011F4:	// 40011f4: <20420010>	ADDI      V0 = V0 + 0010
	state.r[V0] = state.r[V0] + 0x0010;


_040011F8:	// 40011f8: <20010010>	ADDI      AT = R0 + 0010
	state.r[AT] = 0 + 0x0010;


_040011FC:	// 40011fc: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x02AF;
	sp_reg_pc = 0x040011FC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001200:	// 4001200: <200302af>	ADDI      V1 = R0 + 02af
	state.r[V1] = 0 + 0x02AF;


_04001204:	// 4001204: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x04001204;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001208:	// 4001208: <00000000>	NOP       


_0400120C:	// 400120c: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001210:	// 4001210: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001214:	// 4001214: <8401000e>	LH        AT = [R0+000e]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);


_04001218:	// 4001218: <8c020008>	LW        V0 = [R0+0008]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000008) & 0xfff);


_0400121C:	// 400121c: <0d00045f>	JAL	    0400117c
	state.r[V1] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000C) & 0xfff);
	sp_reg_pc = 0x0400121C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001220:	// 4001220: <8403000c>	LH        V1 = [R0+000c]
	state.r[V1] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000C) & 0xfff);


_04001224:	// 4001224: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x04001224;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001228:	// 4001228: <00000000>	NOP       


_0400122C:	// 400122c: <0900048e>	J         04001238
	goto _04001238;


_04001230:	// 4001230: <00000000>	NOP       


_04001234:	// 4001234: <00000000>	NOP       


_04001238:	// 4001238: <00000000>	NOP       


_0400123C:	// 400123c: <00000000>	NOP       


_04001240:	// 4001240: <3322ffff>	ANDI      V0 = T9 & ffff
	state.r[V0] = (state.r[T9] & 0x0000FFFF);


_04001244:	// 4001244: <4a01086c>	VXOR      vec01 = vec01 xor vec01[<none>]
	rsp_cop2_vxor(&state.v[1], &state.v[1], &state.v[1]);


_04001248:	// 4001248: <3341ffff>	ANDI      AT = K0 & ffff
	state.r[AT] = (state.r[K0] & 0x0000FFFF);


_0400124C:	// 400124c: <20210500>	ADDI      AT = AT + 0500
	state.r[AT] = state.r[AT] + 0x0500;


_04001250:	// 4001250: <e8211800>	SDV       [AT + 0x00] = vec01 <00>
	// In function: SmashBros11AC_11B4
	//Call the function: SmashBros11AC_11B4
	SmashBros11AC_11B4();
	goto _0400125C;

_04001254:	// 4001254: <e8211801>	SDV       [AT + 0x04] = vec01 <00>
	ErrorToCallFunction(0x04001254);

_04001258:	// 4001258: <2042fff0>	ADDI      V0 = V0 + fff0
	ErrorToCallFunction(0x04001258);

_0400125C:	// 400125c: <1c40fffc>	BGTZ      (R0>0) --> 1250
	sp_reg_pc = 0x0400125C;
	if((_s32)state.r[V0] > 0)
	{
		state.r[AT] = state.r[AT] + 0x0010;
		goto _04001250;
	}


_04001260:	// 4001260: <20210010>	ADDI      AT = AT + 0010
	state.r[AT] = state.r[AT] + 0x0010;


_04001264:	// 4001264: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001268:	// 4001268: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400126C:	// 400126c: <001a1a00>	SLL       V1 = K0 << 8
	state.r[V1] = ((_u32)state.r[K0] << 8);


_04001270:	// 4001270: <00031d02>	SRL       V1 = V1 >> 20
	state.r[V1] = ((_u32)state.r[V1] >> 20);


_04001274:	// 4001274: <1060ffa5>	BEQ       (R0==V1) --> 110c
	sp_reg_pc = 0x04001274;
	if (state.r[V1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _0400110C;
	}


_04001278:	// 4001278: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400127C:	// 400127c: <33410fff>	ANDI      AT = K0 & 0fff
	state.r[AT] = (state.r[K0] & 0x00000FFF);


_04001280:	// 4001280: <20210500>	ADDI      AT = AT + 0500
	state.r[AT] = state.r[AT] + 0x0500;


_04001284:	// 4001284: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001288:	// 4001288: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_0400128C:	// 400128c: <0d00045f>	JAL	    0400117c
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x0400128C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001290:	// 4001290: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001294:	// 4001294: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001298:	// 4001298: <1420ffff>	BNE       (R0!=AT) --> 1298
	sp_reg_pc = 0x04001298;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _04001298;
	}


_0400129C:	// 400129c: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040012A0:	// 40012a0: <09000443>	J         0400110c
	goto _0400110C;


_040012A4:	// 40012a4: <00000000>	NOP       


_040012A8:	// 40012a8: <001a1a00>	SLL       V1 = K0 << 8
	state.r[V1] = ((_u32)state.r[K0] << 8);


_040012AC:	// 40012ac: <00031d02>	SRL       V1 = V1 >> 20
	state.r[V1] = ((_u32)state.r[V1] >> 20);


_040012B0:	// 40012b0: <1060ff96>	BEQ       (R0==V1) --> 110c
	sp_reg_pc = 0x040012B0;
	if (state.r[V1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _0400110C;
	}


_040012B4:	// 40012b4: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040012B8:	// 40012b8: <33410fff>	ANDI      AT = K0 & 0fff
	state.r[AT] = (state.r[K0] & 0x00000FFF);


_040012BC:	// 40012bc: <20210500>	ADDI      AT = AT + 0500
	state.r[AT] = state.r[AT] + 0x0500;


_040012C0:	// 40012c0: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_040012C4:	// 40012c4: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040012C8:	// 40012c8: <0d000466>	JAL	    04001198
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x040012C8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_040012CC:	// 40012cc: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040012D0:	// 40012d0: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040012D4:	// 40012d4: <1420ffff>	BNE       (R0!=AT) --> 12d4
	sp_reg_pc = 0x040012D4;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _040012D4;
	}


_040012D8:	// 40012d8: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_040012DC:	// 40012dc: <09000443>	J         0400110c
	goto _0400110C;


_040012E0:	// 40012e0: <00000000>	NOP       


_040012E4:	// 40012e4: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_040012E8:	// 40012e8: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040012EC:	// 40012ec: <20010400>	ADDI      AT = R0 + 0400
	state.r[AT] = 0 + 0x0400;


_040012F0:	// 40012f0: <3343ffff>	ANDI      V1 = K0 & ffff
	state.r[V1] = (state.r[K0] & 0x0000FFFF);


_040012F4:	// 40012f4: <0d00045f>	JAL	    0400117c
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x040012F4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_040012F8:	// 40012f8: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040012FC:	// 40012fc: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001300:	// 4001300: <1420ffff>	BNE       (R0!=AT) --> 1300
	sp_reg_pc = 0x04001300;
	if (state.r[AT] != 0)
	{
		state.r[AT] = 0;
		goto _04001300;
	}


_04001304:	// 4001304: <40013000>	MFC0      A2 = SP DRAM DMA address
	state.r[AT] = 0;


_04001308:	// 4001308: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_0400130C:	// 400130c: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001310:	// 4001310: <001a1c02>	SRL       V1 = K0 >> 16
	state.r[V1] = ((_u32)state.r[K0] >> 16);


_04001314:	// 4001314: <30610004>	ANDI      AT = V1 & 0004
	state.r[AT] = (state.r[V1] & 0x00000004);


_04001318:	// 4001318: <10200009>	BEQ       (R0==AT) --> 1340
	sp_reg_pc = 0x04001318;
	if (state.r[AT] == 0)
	{
		state.r[AT] = (state.r[V1] & 0x00000002);
		goto _04001340;
	}


_0400131C:	// 400131c: <30610002>	ANDI      AT = V1 & 0002
	state.r[AT] = (state.r[V1] & 0x00000002);


_04001320:	// 4001320: <10200004>	BEQ       (R0==AT) --> 1334
	sp_reg_pc = 0x04001320;
	if (state.r[AT] == 0)
	{
		state.r[V0] = ((_u32)state.r[T9] >> 16);
		goto _04001334;
	}


_04001324:	// 4001324: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_04001328:	// 4001328: <a71a0050>	SH        [T8+0050] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000050) & 0xfff);


_0400132C:	// 400132c: <a702004c>	SH        [T8+004c] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x0000004C) & 0xfff);


_04001330:	// 4001330: <a719004e>	SH        [T8+004e] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x0000004E) & 0xfff);


_04001334:	// 4001334: <a71a0046>	SH        [T8+0046] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000046) & 0xfff);


_04001338:	// 4001338: <a7020048>	SH        [T8+0048] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000048) & 0xfff);


_0400133C:	// 400133c: <a719004a>	SH        [T8+004a] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x0000004A) & 0xfff);


_04001340:	// 4001340: <00191402>	SRL       V0 = T9 >> 16
	state.r[V0] = ((_u32)state.r[T9] >> 16);


_04001344:	// 4001344: <a71a0040>	SH        [T8+0040] = K0
	Save16_DMEM((_u16)state.r[K0], (state.r[T8] + 0x00000040) & 0xfff);


_04001348:	// 4001348: <a7020042>	SH        [T8+0042] = V0
	Save16_DMEM((_u16)state.r[V0], (state.r[T8] + 0x00000042) & 0xfff);


_0400134C:	// 400134c: <a7190044>	SH        [T8+0044] = T9
	Save16_DMEM((_u16)state.r[T9], (state.r[T8] + 0x00000044) & 0xfff);


_04001350:	// 4001350: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001354:	// 4001354: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001358:	// 4001358: <20010170>	ADDI      AT = R0 + 0170
	state.r[AT] = 0 + 0x0170;


_0400135C:	// 400135c: <20040500>	ADDI      A0 = R0 + 0500
	state.r[A0] = 0 + 0x0500;


_04001360:	// 4001360: <200209e0>	ADDI      V0 = R0 + 09e0
	state.r[V0] = 0 + 0x09E0;


_04001364:	// 4001364: <20030b50>	ADDI      V1 = R0 + 0b50
	state.r[V1] = 0 + 0x0B50;


_04001368:	// 4001368: <c8412000>	LQV       vec01 <00> = [V0 + 0x00]
	// In function: SmashBros12E4_1334
	//Call the function: SmashBros12E4_1334
	SmashBros12E4_1334();
	goto _040013BC;

_0400136C:	// 400136c: <c8622000>	LQV       vec02 <00> = [V1 + 0x00]
	ErrorToCallFunction(0x0400136C);

_04001370:	// 4001370: <2021fff0>	ADDI      AT = AT + fff0
	ErrorToCallFunction(0x04001370);

_04001374:	// 4001374: <20420010>	ADDI      V0 = V0 + 0010
	ErrorToCallFunction(0x04001374);

_04001378:	// 4001378: <20630010>	ADDI      V1 = V1 + 0010
	ErrorToCallFunction(0x04001378);

_0400137C:	// 400137c: <e8810800>	SSV       [A0 + 0x00] = vec01 <00>
	ErrorToCallFunction(0x0400137C);

_04001380:	// 4001380: <e8820801>	SSV       [A0 + 0x02] = vec02 <00>
	ErrorToCallFunction(0x04001380);

_04001384:	// 4001384: <e8810902>	SSV       [A0 + 0x04] = vec01 <02>
	ErrorToCallFunction(0x04001384);

_04001388:	// 4001388: <e8820903>	SSV       [A0 + 0x06] = vec02 <02>
	ErrorToCallFunction(0x04001388);

_0400138C:	// 400138c: <e8810a04>	SSV       [A0 + 0x08] = vec01 <04>
	ErrorToCallFunction(0x0400138C);

_04001390:	// 4001390: <e8820a05>	SSV       [A0 + 0x0a] = vec02 <04>
	ErrorToCallFunction(0x04001390);

_04001394:	// 4001394: <e8810b06>	SSV       [A0 + 0x0c] = vec01 <06>
	ErrorToCallFunction(0x04001394);

_04001398:	// 4001398: <e8820b07>	SSV       [A0 + 0x0e] = vec02 <06>
	ErrorToCallFunction(0x04001398);

_0400139C:	// 400139c: <e8810c08>	SSV       [A0 + 0x10] = vec01 <08>
	ErrorToCallFunction(0x0400139C);

_040013A0:	// 40013a0: <e8820c09>	SSV       [A0 + 0x12] = vec02 <08>
	ErrorToCallFunction(0x040013A0);

_040013A4:	// 40013a4: <e8810d0a>	SSV       [A0 + 0x14] = vec01 <10>
	ErrorToCallFunction(0x040013A4);

_040013A8:	// 40013a8: <e8820d0b>	SSV       [A0 + 0x16] = vec02 <10>
	ErrorToCallFunction(0x040013A8);

_040013AC:	// 40013ac: <e8810e0c>	SSV       [A0 + 0x18] = vec01 <12>
	ErrorToCallFunction(0x040013AC);

_040013B0:	// 40013b0: <e8820e0d>	SSV       [A0 + 0x1a] = vec02 <12>
	ErrorToCallFunction(0x040013B0);

_040013B4:	// 40013b4: <e8810f0e>	SSV       [A0 + 0x1c] = vec01 <14>
	ErrorToCallFunction(0x040013B4);

_040013B8:	// 40013b8: <e8820f0f>	SSV       [A0 + 0x1e] = vec02 <14>
	ErrorToCallFunction(0x040013B8);

_040013BC:	// 40013bc: <1c20ffea>	BGTZ      (R0>0) --> 1368
	sp_reg_pc = 0x040013BC;
	if((_s32)state.r[AT] > 0)
	{
		state.r[A0] = state.r[A0] + 0x0020;
		goto _04001368;
	}


_040013C0:	// 40013c0: <20840020>	ADDI      A0 = A0 + 0020
	state.r[A0] = state.r[A0] + 0x0020;


_040013C4:	// 40013c4: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_040013C8:	// 40013c8: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040013CC:	// 40013cc: <3321ffff>	ANDI      AT = T9 & ffff
	state.r[AT] = (state.r[T9] & 0x0000FFFF);


_040013D0:	// 40013d0: <3342ffff>	ANDI      V0 = K0 & ffff
	state.r[V0] = (state.r[K0] & 0x0000FFFF);


_040013D4:	// 40013d4: <20420500>	ADDI      V0 = V0 + 0500
	state.r[V0] = state.r[V0] + 0x0500;


_040013D8:	// 40013d8: <00191c02>	SRL       V1 = T9 >> 16
	state.r[V1] = ((_u32)state.r[T9] >> 16);


_040013DC:	// 40013dc: <20630500>	ADDI      V1 = V1 + 0500
	state.r[V1] = state.r[V1] + 0x0500;


_040013E0:	// 40013e0: <c8411800>	LDV       vec01 <00> = [V0 + 0x00]
	// In function: Mario1424_1438
	//Call the function: Mario1424_1438
	Mario1424_1438();
	goto _040013F8;

_040013E4:	// 40013e4: <c8421801>	LDV       vec02 <00> = [V0 + 0x08]
	ErrorToCallFunction(0x040013E4);

_040013E8:	// 40013e8: <2021fff0>	ADDI      AT = AT + fff0
	ErrorToCallFunction(0x040013E8);

_040013EC:	// 40013ec: <20420010>	ADDI      V0 = V0 + 0010
	ErrorToCallFunction(0x040013EC);

_040013F0:	// 40013f0: <e8611800>	SDV       [V1 + 0x00] = vec01 <00>
	ErrorToCallFunction(0x040013F0);

_040013F4:	// 40013f4: <e8621801>	SDV       [V1 + 0x04] = vec02 <00>
	ErrorToCallFunction(0x040013F4);

_040013F8:	// 40013f8: <1c20fff9>	BGTZ      (R0>0) --> 13e0
	sp_reg_pc = 0x040013F8;
	if((_s32)state.r[AT] > 0)
	{
		state.r[V1] = state.r[V1] + 0x0010;
		goto _040013E0;
	}


_040013FC:	// 40013fc: <20630010>	ADDI      V1 = V1 + 0010
	state.r[V1] = state.r[V1] + 0x0010;


_04001400:	// 4001400: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001404:	// 4001404: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001408:	// 4001408: <00190a00>	SLL       AT = T9 << 8
	state.r[AT] = ((_u32)state.r[T9] << 8);


_0400140C:	// 400140c: <00010a02>	SRL       AT = AT >> 8
	state.r[AT] = ((_u32)state.r[AT] >> 8);


_04001410:	// 4001410: <ac010ffc>	SW        [R0+0ffc] = AT
	Save32_DMEM((_u32)state.r[AT], (0 + 0x00000FFC) & 0xfff);


_04001414:	// 4001414: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001418:	// 4001418: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400141C:	// 400141c: <00190a00>	SLL       AT = T9 << 8
	state.r[AT] = ((_u32)state.r[T9] << 8);


_04001420:	// 4001420: <00010a02>	SRL       AT = AT >> 8
	state.r[AT] = ((_u32)state.r[AT] >> 8);


_04001424:	// 4001424: <ac010ff4>	SW        [R0+0ff4] = AT
	Save32_DMEM((_u32)state.r[AT], (0 + 0x00000FF4) & 0xfff);


_04001428:	// 4001428: <001a1200>	SLL       V0 = K0 << 8
	state.r[V0] = ((_u32)state.r[K0] << 8);


_0400142C:	// 400142c: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001430:	// 4001430: <ac020ff0>	SW        [R0+0ff0] = V0
	Save32_DMEM((_u32)state.r[V0], (0 + 0x00000FF0) & 0xfff);


_04001434:	// 4001434: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001438:	// 4001438: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_0400143C:	// 400143c: <c81f2006>	LQV       vec31 <00> = [R0 + 0x60]
	{
		_u32 addr = (0x00000060 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001440:	// 4001440: <0019bb02>	SRL       S7 = T9 >> 12
	state.r[S7] = ((_u32)state.r[T9] >> 12);


_04001444:	// 4001444: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_04001448:	// 4001448: <32f7000f>	ANDI      S7 = S7 & 000f
	state.r[S7] = (state.r[S7] & 0x0000000F);


_0400144C:	// 400144c: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_04001450:	// 4001450: <22f70500>	ADDI      S7 = S7 + 0500
	state.r[S7] = state.r[S7] + 0x0500;


_04001454:	// 4001454: <4a0d6b6c>	VXOR      vec13 = vec13 xor vec13[<none>]
	rsp_cop2_vxor(&state.v[13], &state.v[13], &state.v[13]);


_04001458:	// 4001458: <33210fff>	ANDI      AT = T9 & 0fff
	state.r[AT] = (state.r[T9] & 0x00000FFF);


_0400145C:	// 400145c: <4a0e73ac>	VXOR      vec14 = vec14 xor vec14[<none>]
	rsp_cop2_vxor(&state.v[14], &state.v[14], &state.v[14]);


_04001460:	// 4001460: <20210500>	ADDI      AT = AT + 0500
	state.r[AT] = state.r[AT] + 0x0500;


_04001464:	// 4001464: <4a0f7bec>	VXOR      vec15 = vec15 xor vec15[<none>]
	rsp_cop2_vxor(&state.v[15], &state.v[15], &state.v[15]);


_04001468:	// 4001468: <0019ac02>	SRL       S5 = T9 >> 16
	state.r[S5] = ((_u32)state.r[T9] >> 16);


_0400146C:	// 400146c: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_04001470:	// 4001470: <32b50fff>	ANDI      S5 = S5 & 0fff
	state.r[S5] = (state.r[S5] & 0x00000FFF);


_04001474:	// 4001474: <4a118c6c>	VXOR      vec17 = vec17 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[17], &state.v[17]);


_04001478:	// 4001478: <001aa200>	SLL       S4 = K0 << 8
	state.r[S4] = ((_u32)state.r[K0] << 8);


_0400147C:	// 400147c: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_04001480:	// 4001480: <0014a202>	SRL       S4 = S4 >> 8
	state.r[S4] = ((_u32)state.r[S4] >> 8);


_04001484:	// 4001484: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001488:	// 4001488: <2003001f>	ADDI      V1 = R0 + 001f
	state.r[V1] = 0 + 0x001F;


_0400148C:	// 400148c: <00196f02>	SRL       T5 = T9 >> 28
	state.r[T5] = ((_u32)state.r[T9] >> 28);


_04001490:	// 4001490: <31a20001>	ANDI      V0 = T5 & 0001
	state.r[V0] = (state.r[T5] & 0x00000001);


_04001494:	// 4001494: <1c400017>	BGTZ      (R0>0) --> 14f4
	sp_reg_pc = 0x04001494;
	if((_s32)state.r[V0] > 0)
	{
		state.r[S6] = state.r[S7] + 0x0001;
		goto _040014F4;
	}


_04001498:	// 4001498: <22f60001>	ADDI      S6 = S7 + 0001
	state.r[S6] = state.r[S7] + 0x0001;


_0400149C:	// 400149c: <31a20002>	ANDI      V0 = T5 & 0002
	state.r[V0] = (state.r[T5] & 0x00000002);


_040014A0:	// 40014a0: <10400002>	BEQ       (R0==V0) --> 14ac
	sp_reg_pc = 0x040014A0;
	if (state.r[V0] == 0)
	{
		state.r[V0] = state.r[S4] + 0x0000;
		goto _040014AC;
	}


_040014A4:	// 40014a4: <22820000>	ADDI      V0 = S4 + 0000
	state.r[V0] = state.r[S4] + 0x0000;


_040014A8:	// 40014a8: <8c020ffc>	LW        V0 = [R0+0ffc]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000FFC) & 0xfff);


_040014AC:	// 40014ac: <400d2800>	MFC0      A1 = DP buffer busy counter
	state.r[T5] = 0;


_040014B0:	// 40014b0: <15a0ffff>	BNE       (R0!=T5) --> 14b0
	sp_reg_pc = 0x040014B0;
	if (state.r[T5] != 0)
	{
		state.r[T5] = 0;
		goto _040014B0;
	}


_040014B4:	// 40014b4: <400d2800>	MFC0      A1 = DP buffer busy counter
	state.r[T5] = 0;


_040014B8:	// 40014b8: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040014BC:	// 40014bc: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040014C0:	// 40014c0: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040014C0;
	DMARead(state.r[V1]);


_040014C4:	// 40014c4: <20130030>	ADDI      S3 = R0 + 0030
	state.r[S3] = 0 + 0x0030;


_040014C8:	// 40014c8: <20120400>	ADDI      S2 = R0 + 0400
	state.r[S2] = 0 + 0x0400;


_040014CC:	// 40014cc: <ca791800>	LDV       vec25 <00> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 25, 8);


_040014D0:	// 40014d0: <ca781c00>	LDV       vec24 <08> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 24, 0);


_040014D4:	// 40014d4: <ca771801>	LDV       vec23 <00> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 8);


_040014D8:	// 40014d8: <ca771c01>	LDV       vec23 <08> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 0);


_040014DC:	// 40014dc: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040014E0:	// 40014e0: <14a0ffff>	BNE       (R0!=A1) --> 14e0
	sp_reg_pc = 0x040014E0;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _040014E0;
	}


_040014E4:	// 40014e4: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040014E8:	// 40014e8: <00000020>	ADD       R0 = R0+R0
	; //0 = (0 + 0);



_040014EC:	// 40014ec: <09000546>	J         04001518
	{
		_u32 addr = (0x00000010 + state.r[AT]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}
	goto _04001518;


_040014F0:	// 40014f0: <c83b2001>	LQV       vec27 <00> = [AT + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[AT]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_040014F4:	// 40014f4: <20130030>	ADDI      S3 = R0 + 0030
	state.r[S3] = 0 + 0x0030;


_040014F8:	// 40014f8: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_040014FC:	// 40014fc: <20120400>	ADDI      S2 = R0 + 0400
	state.r[S2] = 0 + 0x0400;


_04001500:	// 4001500: <ca791800>	LDV       vec25 <00> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 25, 8);


_04001504:	// 4001504: <ca781c00>	LDV       vec24 <08> = [S3 + 0x00]
	rsp_ldv_compiler((state.r[S3] & 0xFFF), 24, 0);


_04001508:	// 4001508: <ca771801>	LDV       vec23 <00> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 8);


_0400150C:	// 400150c: <ca771c01>	LDV       vec23 <08> = [S3 + 0x08]
	rsp_ldv_compiler((state.r[S3] + 0x00000008) & 0xFFF, 23, 0);


_04001510:	// 4001510: <e83b2000>	SQV       [AT + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[AT]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001514:	// 4001514: <e83b2001>	SQV       [AT + 0x10] = vec27 <00>
	{
		_u32    addr = (0x00000010 + state.r[AT]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001518:	// 4001518: <12a0006b>	BEQ       (R0==S5) --> 16c8
	sp_reg_pc = 0x04001518;
	if (state.r[S5] == 0)
	{
		state.r[AT] = state.r[AT] + 0x0020;
		goto _040016C8;
	}


_0400151C:	// 400151c: <20210020>	ADDI      AT = AT + 0020
	state.r[AT] = state.r[AT] + 0x0020;


_04001520:	// 4001520: <cacc1800>	LDV       vec12 <00> = [S6 + 0x00]
	rsp_ldv_compiler((state.r[S6] & 0xFFF), 12, 8);


_04001524:	// 4001524: <92ea0000>	LBU       T2 = [S7+0000]
	state.r[T2] = (_s32)(_u8)pDMEM[((state.r[S7] + 0x00000000) & 0xfff) ^ 3];


_04001528:	// 4001528: <200d000c>	ADDI      T5 = R0 + 000c
	state.r[T5] = 0 + 0x000C;


_0400152C:	// 400152c: <200c0001>	ADDI      T4 = R0 + 0001
	state.r[T4] = 0 + 0x0001;


_04001530:	// 4001530: <314e000f>	ANDI      T6 = T2 & 000f
	state.r[T6] = (state.r[T2] & 0x0000000F);


_04001534:	// 4001534: <000e7140>	SLL       T6 = T6 << 5
	state.r[T6] = ((_u32)state.r[T6] << 5);


_04001538:	// 4001538: <4b0ccaa8>	VAND      vec10 = vec25 and vec12[0]
	rsp_cop2_vand_element(&state.v[10], &state.v[25], &state.v[12].U16[7]);


_0400153C:	// 400153c: <01d28020>	ADD       S0 = T6+S2
	state.r[S0] = (state.r[T6] + state.r[S2]);


_04001540:	// 4001540: <4b2cc268>	VAND      vec09 = vec24 and vec12[1]
	rsp_cop2_vand_element(&state.v[9], &state.v[24], &state.v[12].U16[6]);


_04001544:	// 4001544: <000a8902>	SRL       S1 = T2 >> 4
	state.r[S1] = ((_u32)state.r[T2] >> 4);


_04001548:	// 4001548: <4b4cca28>	VAND      vec08 = vec25 and vec12[2]
	rsp_cop2_vand_element(&state.v[8], &state.v[25], &state.v[12].U16[5]);


_0400154C:	// 400154c: <01b18822>	SUB       S1 = T5-S1
	state.r[S1] = (state.r[T5] - state.r[S1]);


_04001550:	// 4001550: <4b6cc1e8>	VAND      vec07 = vec24 and vec12[3]
	rsp_cop2_vand_element(&state.v[7], &state.v[24], &state.v[12].U16[4]);


_04001554:	// 4001554: <222dffff>	ADDI      T5 = S1 + ffff
	state.r[T5] = state.r[S1] + 0xFFFFFFFF;


_04001558:	// 4001558: <000c63c0>	SLL       T4 = T4 << 15
	state.r[T4] = ((_u32)state.r[T4] << 15);


_0400155C:	// 400155c: <01ac5806>	SRLV      T4 = T3 >> T5
	state.r[T3] = ((_u32)state.r[T4] >> (state.r[T5] & 0x1F));


_04001560:	// 4001560: <488bb000>	MTC2      vec22 <00> = T3
	state.v[22].U16[7] = (_u16)state.r[T3];


_04001564:	// 4001564: <ca152000>	LQV       vec21 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


_04001568:	// 4001568: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


_0400156C:	// 400156c: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_04001570:	// 4001570: <ca132802>	LRV       vec19 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[19].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001574:	// 4001574: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_04001578:	// 4001578: <ca122802>	LRV       vec18 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[18].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400157C:	// 400157c: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_04001580:	// 4001580: <ca112802>	LRV       vec17 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[17].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001584:	// 4001584: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_04001588:	// 4001588: <ca102802>	LRV       vec16 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[16].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400158C:	// 400158c: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_04001590:	// 4001590: <ca0f2802>	LRV       vec15 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001594:	// 4001594: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_04001598:	// 4001598: <ca0e2802>	LRV       vec14 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[14].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400159C:	// 400159c: <2210fffe>	ADDI      S0 = S0 + fffe
	state.r[S0] = state.r[S0] + 0xFFFFFFFE;


_040015A0:	// 40015a0: <ca0d2802>	LRV       vec13 <00> = [S0 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[13].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040015A4:	// 40015a4: <22d60009>	ADDI      S6 = S6 + 0009
	// In function: SmashBros1514_1630
	//Call the function: SmashBros1514_1630
	SmashBros1514_1630();
	goto _040016C8;

_040015A8:	// 40015a8: <4a175786>	VMUDN     vec30 = ( acc = vec10 * vec23[<none>]      ) >> 16
	ErrorToCallFunction(0x040015A8);

_040015AC:	// 40015ac: <22f70009>	ADDI      S7 = S7 + 0009
	ErrorToCallFunction(0x040015AC);

_040015B0:	// 40015b0: <4a174f8e>	VMADN     vec30 = ( acc+= vec09 * vec23[<none>] )
	ErrorToCallFunction(0x040015B0);

_040015B4:	// 40015b4: <92ea0000>	LBU       T2 = [S7+0000]
	ErrorToCallFunction(0x040015B4);

_040015B8:	// 40015b8: <4a174746>	VMUDN     vec29 = ( acc = vec08 * vec23[<none>]      ) >> 16
	ErrorToCallFunction(0x040015B8);

_040015BC:	// 40015bc: <cacc1800>	LDV       vec12 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x040015BC);

_040015C0:	// 40015c0: <4a173f4e>	VMADN     vec29 = ( acc+= vec07 * vec23[<none>] )
	ErrorToCallFunction(0x040015C0);

_040015C4:	// 40015c4: <200d000c>	ADDI      T5 = R0 + 000c
	ErrorToCallFunction(0x040015C4);

_040015C8:	// 40015c8: <1a200003>	BLEZ      (R0<=0) --> 15d8
	ErrorToCallFunction(0x040015C8);

_040015CC:	// 40015cc: <314e000f>	ANDI      T6 = T2 & 000f
	ErrorToCallFunction(0x040015CC);

_040015D0:	// 40015d0: <4b16f785>	VMUDM     vec30 = ( acc = vec30 * vec22[0] ) >> 16
	ErrorToCallFunction(0x040015D0);

_040015D4:	// 40015d4: <4b16ef45>	VMUDM     vec29 = ( acc = vec29 * vec22[0] ) >> 16
	ErrorToCallFunction(0x040015D4);

_040015D8:	// 40015d8: <000e7140>	SLL       T6 = T6 << 5
	ErrorToCallFunction(0x040015D8);

_040015DC:	// 40015dc: <4bdbaac7>	VMUDH     vec11 = ( acc = (vec21 * vec27[6]) << 16) >> 16
	ErrorToCallFunction(0x040015DC);

_040015E0:	// 40015e0: <01d28020>	ADD       S0 = T6+S2
	ErrorToCallFunction(0x040015E0);

_040015E4:	// 40015e4: <4bfba2cf>	VMADH     vec11 = ( acc+= (vec20 * vec27[7]) << 16) >> 16
	ErrorToCallFunction(0x040015E4);

_040015E8:	// 40015e8: <4b1e9acf>	VMADH     vec11 = ( acc+= (vec19 * vec30[0]) << 16) >> 16
	ErrorToCallFunction(0x040015E8);

_040015EC:	// 40015ec: <4b3e92cf>	VMADH     vec11 = ( acc+= (vec18 * vec30[1]) << 16) >> 16
	ErrorToCallFunction(0x040015EC);

_040015F0:	// 40015f0: <000a8902>	SRL       S1 = T2 >> 4
	ErrorToCallFunction(0x040015F0);

_040015F4:	// 40015f4: <4b5e8acf>	VMADH     vec11 = ( acc+= (vec17 * vec30[2]) << 16) >> 16
	ErrorToCallFunction(0x040015F4);

_040015F8:	// 40015f8: <4b7e82cf>	VMADH     vec11 = ( acc+= (vec16 * vec30[3]) << 16) >> 16
	ErrorToCallFunction(0x040015F8);

_040015FC:	// 40015fc: <01b18822>	SUB       S1 = T5-S1
	ErrorToCallFunction(0x040015FC);

_04001600:	// 4001600: <4b9e7f0f>	VMADH     vec28 = ( acc+= (vec15 * vec30[4]) << 16) >> 16
	ErrorToCallFunction(0x04001600);

_04001604:	// 4001604: <222dffff>	ADDI      T5 = S1 + ffff
	ErrorToCallFunction(0x04001604);

_04001608:	// 4001608: <4bbe72cf>	VMADH     vec11 = ( acc+= (vec14 * vec30[5]) << 16) >> 16
	ErrorToCallFunction(0x04001608);

_0400160C:	// 400160c: <4bde6acf>	VMADH     vec11 = ( acc+= (vec13 * vec30[6]) << 16) >> 16
	ErrorToCallFunction(0x0400160C);

_04001610:	// 4001610: <4b7ff2cf>	VMADH     vec11 = ( acc+= (vec30 * vec31[3]) << 16) >> 16
	ErrorToCallFunction(0x04001610);

_04001614:	// 4001614: <01ac5806>	SRLV      T4 = T3 >> T5
	ErrorToCallFunction(0x04001614);

_04001618:	// 4001618: <4b3c369d>	VSAW $v26, $v6, $v28[[1]]
	ErrorToCallFunction(0x04001618);

_0400161C:	// 400161c: <488bb000>	MTC2      vec22 <00> = T3
	ErrorToCallFunction(0x0400161C);

_04001620:	// 4001620: <4b1c371d>	VSAW $v28, $v6, $v28[[0]]
	ErrorToCallFunction(0x04001620);

_04001624:	// 4001624: <4b0ccaa8>	VAND      vec10 = vec25 and vec12[0]
	ErrorToCallFunction(0x04001624);

_04001628:	// 4001628: <4b2cc268>	VAND      vec09 = vec24 and vec12[1]
	ErrorToCallFunction(0x04001628);

_0400162C:	// 400162c: <4b4cca28>	VAND      vec08 = vec25 and vec12[2]
	ErrorToCallFunction(0x0400162C);

_04001630:	// 4001630: <4b6cc1e8>	VAND      vec07 = vec24 and vec12[3]
	ErrorToCallFunction(0x04001630);

_04001634:	// 4001634: <4b3fd2c6>	VMUDN     vec11 = ( acc = vec26 * vec31[1]      ) >> 16
	ErrorToCallFunction(0x04001634);

_04001638:	// 4001638: <4b3fe70f>	VMADH     vec28 = ( acc+= (vec28 * vec31[1]) << 16) >> 16
	ErrorToCallFunction(0x04001638);

_0400163C:	// 400163c: <4b1d9ac7>	VMUDH     vec11 = ( acc = (vec19 * vec29[0]) << 16) >> 16
	ErrorToCallFunction(0x0400163C);

_04001640:	// 4001640: <220ffffe>	ADDI      T7 = S0 + fffe
	ErrorToCallFunction(0x04001640);

_04001644:	// 4001644: <4b3d92cf>	VMADH     vec11 = ( acc+= (vec18 * vec29[1]) << 16) >> 16
	ErrorToCallFunction(0x04001644);

_04001648:	// 4001648: <c9f32802>	LRV       vec19 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001648);

_0400164C:	// 400164c: <4b5d8acf>	VMADH     vec11 = ( acc+= (vec17 * vec29[2]) << 16) >> 16
	ErrorToCallFunction(0x0400164C);

_04001650:	// 4001650: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x04001650);

_04001654:	// 4001654: <4b7d82cf>	VMADH     vec11 = ( acc+= (vec16 * vec29[3]) << 16) >> 16
	ErrorToCallFunction(0x04001654);

_04001658:	// 4001658: <c9f22802>	LRV       vec18 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001658);

_0400165C:	// 400165c: <4b9d7acf>	VMADH     vec11 = ( acc+= (vec15 * vec29[4]) << 16) >> 16
	ErrorToCallFunction(0x0400165C);

_04001660:	// 4001660: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x04001660);

_04001664:	// 4001664: <4bbd72cf>	VMADH     vec11 = ( acc+= (vec14 * vec29[5]) << 16) >> 16
	ErrorToCallFunction(0x04001664);

_04001668:	// 4001668: <c9f12802>	LRV       vec17 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001668);

_0400166C:	// 400166c: <4bdd6acf>	VMADH     vec11 = ( acc+= (vec13 * vec29[6]) << 16) >> 16
	ErrorToCallFunction(0x0400166C);

_04001670:	// 4001670: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x04001670);

_04001674:	// 4001674: <4b7feacf>	VMADH     vec11 = ( acc+= (vec29 * vec31[3]) << 16) >> 16
	ErrorToCallFunction(0x04001674);

_04001678:	// 4001678: <c9f02802>	LRV       vec16 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001678);

_0400167C:	// 400167c: <4bdcaacf>	VMADH     vec11 = ( acc+= (vec21 * vec28[6]) << 16) >> 16
	ErrorToCallFunction(0x0400167C);

_04001680:	// 4001680: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x04001680);

_04001684:	// 4001684: <4bfca2cf>	VMADH     vec11 = ( acc+= (vec20 * vec28[7]) << 16) >> 16
	ErrorToCallFunction(0x04001684);

_04001688:	// 4001688: <c9ef2802>	LRV       vec15 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001688);

_0400168C:	// 400168c: <4b3b369d>	VSAW $v26, $v6, $v27[[1]]
	ErrorToCallFunction(0x0400168C);

_04001690:	// 4001690: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x04001690);

_04001694:	// 4001694: <4b1b36dd>	VSAW $v27, $v6, $v27[[0]]
	ErrorToCallFunction(0x04001694);

_04001698:	// 4001698: <c9ee2802>	LRV       vec14 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x04001698);

_0400169C:	// 400169c: <21effffe>	ADDI      T7 = T7 + fffe
	ErrorToCallFunction(0x0400169C);

_040016A0:	// 40016a0: <c9ed2802>	LRV       vec13 <00> = [T7 + 0x20]
	ErrorToCallFunction(0x040016A0);

_040016A4:	// 40016a4: <ca152000>	LQV       vec21 <00> = [S0 + 0x00]
	ErrorToCallFunction(0x040016A4);

_040016A8:	// 40016a8: <4b3fd2c6>	VMUDN     vec11 = ( acc = vec26 * vec31[1]      ) >> 16
	ErrorToCallFunction(0x040016A8);

_040016AC:	// 40016ac: <ca142001>	LQV       vec20 <00> = [S0 + 0x10]
	ErrorToCallFunction(0x040016AC);

_040016B0:	// 40016b0: <4b3fdecf>	VMADH     vec27 = ( acc+= (vec27 * vec31[1]) << 16) >> 16
	ErrorToCallFunction(0x040016B0);

_040016B4:	// 40016b4: <22b5ffe0>	ADDI      S5 = S5 + ffe0
	ErrorToCallFunction(0x040016B4);

_040016B8:	// 40016b8: <e83c2000>	SQV       [AT + 0x00] = vec28 <00>
	ErrorToCallFunction(0x040016B8);

_040016BC:	// 40016bc: <20210020>	ADDI      AT = AT + 0020
	ErrorToCallFunction(0x040016BC);

_040016C0:	// 40016c0: <1ea0ffb8>	BGTZ      (R0>0) --> 15a4
	ErrorToCallFunction(0x040016C0);

_040016C4:	// 40016c4: <e83b207f>	SQV       [AT + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x040016C4);

_040016C8:	// 40016c8: <2021ffe0>	ADDI      AT = AT + ffe0
	state.r[AT] = state.r[AT] + 0xFFFFFFE0;


_040016CC:	// 40016cc: <0d000466>	JAL	    04001198
	state.r[V0] = state.r[S4] + 0x0000;
	sp_reg_pc = 0x040016CC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_040016D0:	// 40016d0: <22820000>	ADDI      V0 = S4 + 0000
	state.r[V0] = state.r[S4] + 0x0000;


_040016D4:	// 40016d4: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_040016D8:	// 40016d8: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040016DC:	// 40016dc: <14a0ffff>	BNE       (R0!=A1) --> 16dc
	sp_reg_pc = 0x040016DC;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _040016DC;
	}


_040016E0:	// 40016e0: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040016E4:	// 40016e4: <09000443>	J         0400110c
	; //0 = (0 & 0);

	goto _0400110C;


_040016E8:	// 40016e8: <00000024>	AND       R0 = R0 & R0
	; //0 = (0 & 0);



_040016EC:	// 40016ec: <20140400>	ADDI      S4 = R0 + 0400
	state.r[S4] = 0 + 0x0400;


_040016F0:	// 40016f0: <4a1ad6ac>	VXOR      vec26 = vec26 xor vec26[<none>]
	rsp_cop2_vxor(&state.v[26], &state.v[26], &state.v[26]);


_040016F4:	// 40016f4: <8e910000>	LW        S1 = [S4+0000]
	state.r[S1] = (_s32)Load32_DMEM((state.r[S4] + 0x00000000) & 0xfff);


_040016F8:	// 40016f8: <1220009c>	BEQ       (R0==S1) --> 196c
	sp_reg_pc = 0x040016F8;
	if (state.r[S1] == 0)
	{
		state.r[S8] = state.r[S8] + 0xFFFFFFF8;
		goto _0400196C;
	}


_040016FC:	// 40016fc: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001700:	// 4001700: <23570500>	ADDI      S7 = K0 + 0500
	state.r[S7] = state.r[K0] + 0x0500;


_04001704:	// 4001704: <ca921800>	LDV       vec18 <00> = [S4 + 0x00]
	rsp_ldv_compiler((state.r[S4] & 0xFFF), 18, 8);


_04001708:	// 4001708: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_0400170C:	// 400170c: <03201020>	ADD       V0 = T9+R0
	state.r[V0] = (state.r[T9] + 0);


_04001710:	// 4001710: <22f5fffc>	ADDI      S5 = S7 + fffc
	state.r[S5] = state.r[S7] + 0xFFFFFFFC;


_04001714:	// 4001714: <22f3fffe>	ADDI      S3 = S7 + fffe
	state.r[S3] = state.r[S7] + 0xFFFFFFFE;


_04001718:	// 4001718: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_0400171C:	// 400171c: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_04001720:	// 4001720: <20030007>	ADDI      V1 = R0 + 0007
	state.r[V1] = 0 + 0x0007;


_04001724:	// 4001724: <4a16b5ac>	VXOR      vec22 = vec22 xor vec22[<none>]
	rsp_cop2_vxor(&state.v[22], &state.v[22], &state.v[22]);


_04001728:	// 4001728: <20160170>	ADDI      S6 = R0 + 0170
	state.r[S6] = 0 + 0x0170;


_0400172C:	// 400172c: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001730:	// 4001730: <001a8402>	SRL       S0 = K0 >> 16
	state.r[S0] = ((_u32)state.r[K0] >> 16);


_04001734:	// 4001734: <4a14a52c>	VXOR      vec20 = vec20 xor vec20[<none>]
	rsp_cop2_vxor(&state.v[20], &state.v[20], &state.v[20]);


_04001738:	// 4001738: <32100001>	ANDI      S0 = S0 & 0001
	state.r[S0] = (state.r[S0] & 0x00000001);


_0400173C:	// 400173c: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001740:	// 4001740: <8e920002>	LW        S2 = [S4+0002]
	state.r[S2] = (_s32)Load32_DMEM((state.r[S4] + 0x00000002) & 0xfff);


_04001744:	// 4001744: <1e00001e>	BGTZ      (R0>0) --> 17c0
	sp_reg_pc = 0x04001744;
	if((_s32)state.r[S0] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _040017C0;
	}


_04001748:	// 4001748: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_0400174C:	// 400174c: <40102800>	MFC0      A1 = unknown
	state.r[S0] = 0;


_04001750:	// 4001750: <1600ffff>	BNE       (R0!=S0) --> 1750
	sp_reg_pc = 0x04001750;
	if (state.r[S0] != 0)
	{
		state.r[S0] = 0;
		goto _04001750;
	}


_04001754:	// 4001754: <40102800>	MFC0      A1 = unknown
	state.r[S0] = 0;


_04001758:	// 4001758: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400175C:	// 400175c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001760:	// 4001760: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001760;
	DMARead(state.r[V1]);


_04001764:	// 4001764: <ca9a1004>	LLV       vec26 <00> = [S4 + 0x10]
	state.v[26].U32[3] = Load32_DMEM(0x00000010 + state.r[S4]);


_04001768:	// 4001768: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400176C:	// 400176c: <ca991802>	LDV       vec25 <00> = [S4 + 0x10]
	rsp_ldv_compiler((state.r[S4] + 0x00000010) & 0xFFF, 25, 8);


_04001770:	// 4001770: <22940002>	ADDI      S4 = S4 + 0002
	state.r[S4] = state.r[S4] + 0x0002;


_04001774:	// 4001774: <ca981204>	LLV       vec24 <04> = [S4 + 0x10]
	state.v[24].U32[2] = Load32_DMEM(0x00000010 + state.r[S4]);


_04001778:	// 4001778: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400177C:	// 400177c: <ca971a02>	LDV       vec23 <04> = [S4 + 0x10]
	rsp_ldv_compiler((state.r[S4] + 0x00000010) & 0xFFF, 23, 8);


_04001780:	// 4001780: <22940002>	ADDI      S4 = S4 + 0002
	state.r[S4] = state.r[S4] + 0x0002;


_04001784:	// 4001784: <ca961404>	LLV       vec22 <08> = [S4 + 0x10]
	state.v[22].U32[1] = Load32_DMEM(0x00000010 + state.r[S4]);


_04001788:	// 4001788: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_0400178C:	// 400178c: <ca951c02>	LDV       vec21 <08> = [S4 + 0x10]
	rsp_ldv_compiler((state.r[S4] + 0x00000010) & 0xFFF, 21, 0);


_04001790:	// 4001790: <22940002>	ADDI      S4 = S4 + 0002
	state.r[S4] = state.r[S4] + 0x0002;


_04001794:	// 4001794: <ca941604>	LLV       vec20 <12> = [S4 + 0x10]
	state.v[20].U32[0] = Load32_DMEM(0x00000010 + state.r[S4]);


_04001798:	// 4001798: <ca930f08>	LSV       vec19 <14> = [S4 + 0x10]
	state.v[19].U16[0] = Load16_DMEM(0x00000010 + state.r[S4]);


_0400179C:	// 400179c: <ca930809>	LSV       vec19 <00> = [S4 + 0x12]
	state.v[19].U16[7] = Load16_DMEM(0x00000012 + state.r[S4]);


_040017A0:	// 40017a0: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040017A4:	// 40017a4: <14a0ffff>	BNE       (R0!=A1) --> 17a4
	sp_reg_pc = 0x040017A4;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _040017A4;
	}


_040017A8:	// 40017a8: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_040017AC:	// 40017ac: <c82f1000>	LLV       vec15 <00> = [AT + 0x00]
	state.v[15].U32[3] = Load32_DMEM(0x00000000 + state.r[AT]);


_040017B0:	// 40017b0: <12400016>	BEQ       (R0==S2) --> 180c
	sp_reg_pc = 0x040017B0;
	if (state.r[S2] == 0)
	{
		state.v[28].U32[0] = Load32_DMEM(0x00000004 + state.r[AT]);
		goto _0400180C;
	}


_040017B4:	// 40017b4: <c83c1601>	LLV       vec28 <12> = [AT + 0x04]
	state.v[28].U32[0] = Load32_DMEM(0x00000004 + state.r[AT]);


_040017B8:	// 40017b8: <0900060b>	J         0400182c
	state.v[11].U16[7] = Load16_DMEM(0x00000002 + state.r[AT]);
	goto _0400182C;


_040017BC:	// 40017bc: <c82b0801>	LSV       vec11 <00> = [AT + 0x02]
	state.v[11].U16[7] = Load16_DMEM(0x00000002 + state.r[AT]);


_040017C0:	// 40017c0: <ca9a1004>	LLV       vec26 <00> = [S4 + 0x10]
	state.v[26].U32[3] = Load32_DMEM(0x00000010 + state.r[S4]);


_040017C4:	// 40017c4: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040017C8:	// 40017c8: <4a0f7bec>	VXOR      vec15 = vec15 xor vec15[<none>]
	rsp_cop2_vxor(&state.v[15], &state.v[15], &state.v[15]);


_040017CC:	// 40017cc: <ca991802>	LDV       vec25 <00> = [S4 + 0x10]
	rsp_ldv_compiler((state.r[S4] + 0x00000010) & 0xFFF, 25, 8);


_040017D0:	// 40017d0: <22940002>	ADDI      S4 = S4 + 0002
	state.r[S4] = state.r[S4] + 0x0002;


_040017D4:	// 40017d4: <4a0b5aec>	VXOR      vec11 = vec11 xor vec11[<none>]
	rsp_cop2_vxor(&state.v[11], &state.v[11], &state.v[11]);


_040017D8:	// 40017d8: <ca981204>	LLV       vec24 <04> = [S4 + 0x10]
	state.v[24].U32[2] = Load32_DMEM(0x00000010 + state.r[S4]);


_040017DC:	// 40017dc: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040017E0:	// 40017e0: <4a1ce72c>	VXOR      vec28 = vec28 xor vec28[<none>]
	rsp_cop2_vxor(&state.v[28], &state.v[28], &state.v[28]);


_040017E4:	// 40017e4: <ca971a02>	LDV       vec23 <04> = [S4 + 0x10]
	rsp_ldv_compiler((state.r[S4] + 0x00000010) & 0xFFF, 23, 8);


_040017E8:	// 40017e8: <22940002>	ADDI      S4 = S4 + 0002
	state.r[S4] = state.r[S4] + 0x0002;


_040017EC:	// 40017ec: <ca961404>	LLV       vec22 <08> = [S4 + 0x10]
	state.v[22].U32[1] = Load32_DMEM(0x00000010 + state.r[S4]);


_040017F0:	// 40017f0: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040017F4:	// 40017f4: <ca951c02>	LDV       vec21 <08> = [S4 + 0x10]
	rsp_ldv_compiler((state.r[S4] + 0x00000010) & 0xFFF, 21, 0);


_040017F8:	// 40017f8: <22940002>	ADDI      S4 = S4 + 0002
	state.r[S4] = state.r[S4] + 0x0002;


_040017FC:	// 40017fc: <ca941604>	LLV       vec20 <12> = [S4 + 0x10]
	state.v[20].U32[0] = Load32_DMEM(0x00000010 + state.r[S4]);


_04001800:	// 4001800: <ca930f08>	LSV       vec19 <14> = [S4 + 0x10]
	state.v[19].U16[0] = Load16_DMEM(0x00000010 + state.r[S4]);


_04001804:	// 4001804: <16400009>	BNE       (R0!=S2) --> 182c
	sp_reg_pc = 0x04001804;
	if (state.r[S2] != 0)
	{
		state.v[19].U16[7] = Load16_DMEM(0x00000012 + state.r[S4]);
		goto _0400182C;
	}


_04001808:	// 4001808: <ca930809>	LSV       vec19 <00> = [S4 + 0x12]
	state.v[19].U16[7] = Load16_DMEM(0x00000012 + state.r[S4]);


_0400180C:	// 400180c: <cafe2000>	LQV       vec30 <00> = [S7 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001810:	// 4001810: <caaf2801>	LRV       vec15 <00> = [S5 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[S5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001814:	// 4001814: <4a0ff7d1>	VSUB      vec31 = vec30 - vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[31].U16[0], state.v[30].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[31].U16[1], state.v[30].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[31].U16[2], state.v[30].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[31].U16[3], state.v[30].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[31].U16[4], state.v[30].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[31].U16[5], state.v[30].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[31].U16[6], state.v[30].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[31].U16[7], state.v[30].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001818:	// 4001818: <4bdc9c00>	VMULF     vec16 = ( acc = (vec19 * vec28[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
	}


_0400181C:	// 400181c: <4b12ffc0>	VMULF     vec31 = ( acc = (vec31 * vec18[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMULF_operation(0, state.v[31].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[31].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[31].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[31].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[31].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[31].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[31].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[31].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


_04001820:	// 4001820: <4a108410>	VADD      vec16 = vec16 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001824:	// 4001824: <09000651>	J         04001944
	state.v[29].U16[0] = state.v[28].U16[0];
	goto _04001944;


_04001828:	// 4001828: <4bfc7f73>	VMOV      vec29[7] = vec28[7]
	state.v[29].U16[0] = state.v[28].U16[0];


_0400182C:	// 400182c: <cafe2000>	LQV       vec30 <00> = [S7 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001830:	// 4001830: <ca6b2801>	LRV       vec11 <00> = [S3 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[S3]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[11].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001834:	// 4001834: <caaf2801>	LRV       vec15 <00> = [S5 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[S5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001838:	// 4001838: <4b12f780>	VMULF     vec30 = ( acc = (vec30 * vec18[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMULF_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 0, 0, 1)
	}


_0400183C:	// 400183c: <4b325ac0>	VMULF     vec11 = ( acc = (vec11 * vec18[1]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[6];
		VMULF_operation(0, state.v[11].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[11].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[11].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[11].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[11].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[11].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[11].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[11].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
	}


_04001840:	// 4001840: <4b127bc0>	VMULF     vec15 = ( acc = (vec15 * vec18[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMULF_operation(0, state.v[15].U16[0], state.v[15].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[15].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[15].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[15].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[15].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[15].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[15].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[15].U16[7], s2value, 1, 0, 0, 1)
	}


_04001844:	// 4001844: <4bdc9c00>	VMULF     vec16 = ( acc = (vec19 * vec28[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
	}


_04001848:	// 4001848: <4a0bf7d0>	VADD      vec31 = vec30 + vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[31].U16[0], state.v[30].U16[0], state.v[11].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[31].U16[1], state.v[30].U16[1], state.v[11].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[31].U16[2], state.v[30].U16[2], state.v[11].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[31].U16[3], state.v[30].U16[3], state.v[11].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[31].U16[4], state.v[30].U16[4], state.v[11].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[31].U16[5], state.v[30].U16[5], state.v[11].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[31].U16[6], state.v[30].U16[6], state.v[11].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[31].U16[7], state.v[30].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400184C:	// 400184c: <4a108410>	VADD      vec16 = vec16 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001850:	// 4001850: <4a0fffd0>	VADD      vec31 = vec31 + vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[31].U16[0], state.v[31].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[31].U16[1], state.v[31].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[31].U16[2], state.v[31].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[31].U16[3], state.v[31].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[31].U16[4], state.v[31].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[31].U16[5], state.v[31].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[31].U16[6], state.v[31].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[31].U16[7], state.v[31].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001854:	// 4001854: <4bfcd440>	VMULF     vec17 = ( acc = (vec26 * vec28[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[0];
		VMULF_operation(0, state.v[17].U16[0], state.v[26].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[17].U16[1], state.v[26].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[17].U16[2], state.v[26].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[17].U16[3], state.v[26].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[17].U16[4], state.v[26].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[17].U16[5], state.v[26].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[17].U16[6], state.v[26].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[17].U16[7], state.v[26].U16[7], s2value, 1, 0, 0, 1)
	}


_04001858:	// 4001858: <4b1f86d0>	VADD      vec27 = vec16 + vec31[0]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[7];
		VADD_operation(0, state.v[27].U16[0], state.v[16].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[16].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[16].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[16].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[16].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[16].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[16].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[16].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400185C:	// 400185c: <4bfc7f73>	VMOV      vec29[7] = vec28[7]
	state.v[29].U16[0] = state.v[28].U16[0];


_04001860:	// 4001860: <22d6fff0>	ADDI      S6 = S6 + fff0
	state.r[S6] = state.r[S6] + 0xFFFFFFF0;


_04001864:	// 4001864: <4a118c50>	VADD      vec17 = vec17 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[17].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[17].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[17].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[17].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[17].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[17].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[17].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[17].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001868:	// 4001868: <4a11df10>	VADD      vec28 = vec27 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[17].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[17].U16[1], 0, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[17].U16[2], 0, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[17].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[17].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[17].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400186C:	// 400186c: <4b3f8ed0>	VADD      vec27 = vec17 + vec31[1]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[6];
		VADD_operation(0, state.v[27].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001870:	// 4001870: <4b1ccc00>	VMULF     vec16 = ( acc = (vec25 * vec28[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[7];
		VMULF_operation(0, state.v[16].U16[0], state.v[25].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[25].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[25].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[25].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[25].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[25].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[25].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[25].U16[7], s2value, 1, 0, 0, 1)
	}


_04001874:	// 4001874: <4a1debaa>	VOR       vec14 = vec29 or vec29[<none>]
	rsp_cop2_vor(&state.v[14], &state.v[29], &state.v[29]);


_04001878:	// 4001878: <ca6b2001>	LQV       vec11 <00> = [S3 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[11].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[11].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[11].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[11].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[11].U64[0] = state.v[11].U64[0] & ~temp1;
			state.v[11].U64[1] = state.v[11].U64[1] & ~temp2;
			state.v[11].U64[0] = state.v[11].U64[0] | value1;
			state.v[11].U64[1] = state.v[11].U64[1] | value2;
		}
	}


_0400187C:	// 400187c: <4b1c4773>	VMOV      vec29[0] = vec28[0]
	state.v[29].U16[7] = state.v[28].U16[7];


_04001880:	// 4001880: <4a108410>	VADD      vec16 = vec16 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001884:	// 4001884: <4a10df10>	VADD      vec28 = vec27 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001888:	// 4001888: <4b5f86d0>	VADD      vec27 = vec16 + vec31[2]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[5];
		VADD_operation(0, state.v[27].U16[0], state.v[16].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[16].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[16].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[16].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[16].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[16].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[16].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[16].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400188C:	// 400188c: <4b3cc440>	VMULF     vec17 = ( acc = (vec24 * vec28[1]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[6];
		VMULF_operation(0, state.v[17].U16[0], state.v[24].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[17].U16[1], state.v[24].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[17].U16[2], state.v[24].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[17].U16[3], state.v[24].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[17].U16[4], state.v[24].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[17].U16[5], state.v[24].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[17].U16[6], state.v[24].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[17].U16[7], state.v[24].U16[7], s2value, 1, 1, 1, 1)
	}


_04001890:	// 4001890: <ca6b2802>	LRV       vec11 <00> = [S3 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S3]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[11].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001894:	// 4001894: <16400002>	BNE       (R0!=S2) --> 18a0
	sp_reg_pc = 0x04001894;
	if (state.r[S2] != 0)
	{
		state.v[29].U16[6] = state.v[28].U16[6];
		goto _040018A0;
	}


_04001898:	// 4001898: <4b3c4f73>	VMOV      vec29[1] = vec28[1]
	state.v[29].U16[6] = state.v[28].U16[6];


_0400189C:	// 400189c: <eaee207f>	SQV       [S7 + 0xf0] = vec14 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040018A0:	// 40018a0: <4a118c50>	VADD      vec17 = vec17 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[17].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[17].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[17].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[17].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[17].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[17].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[17].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[17].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018A4:	// 40018a4: <4a11df10>	VADD      vec28 = vec27 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[17].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[17].U16[1], 0, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[17].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[17].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[17].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[17].U16[7], 0, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018A8:	// 40018a8: <00009025>	OR        S2 = R0 | R0
	state.r[S2] = (0 | 0);


_040018AC:	// 40018ac: <4b7f8ed0>	VADD      vec27 = vec17 + vec31[3]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[4];
		VADD_operation(0, state.v[27].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018B0:	// 40018b0: <4b5cbc00>	VMULF     vec16 = ( acc = (vec23 * vec28[2]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[5];
		VMULF_operation(0, state.v[16].U16[0], state.v[23].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[23].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[23].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[23].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[23].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[23].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[23].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[23].U16[7], s2value, 1, 0, 0, 1)
	}


_040018B4:	// 40018b4: <4b5c5773>	VMOV      vec29[2] = vec28[2]
	state.v[29].U16[5] = state.v[28].U16[5];


_040018B8:	// 40018b8: <4b325ac0>	VMULF     vec11 = ( acc = (vec11 * vec18[1]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[6];
		VMULF_operation(0, state.v[11].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[11].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[11].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[11].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[11].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[11].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[11].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[11].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
	}


_040018BC:	// 40018bc: <cafe2001>	LQV       vec30 <00> = [S7 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_040018C0:	// 40018c0: <4a108410>	VADD      vec16 = vec16 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018C4:	// 40018c4: <4a10df10>	VADD      vec28 = vec27 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[16].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[16].U16[1], 0, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[16].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[16].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[16].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[16].U16[7], 0, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018C8:	// 40018c8: <4b9f86d0>	VADD      vec27 = vec16 + vec31[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[3];
		VADD_operation(0, state.v[27].U16[0], state.v[16].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[16].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[16].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[16].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[16].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[16].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[16].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[16].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018CC:	// 40018cc: <4b7cb440>	VMULF     vec17 = ( acc = (vec22 * vec28[3]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[4];
		VMULF_operation(0, state.v[17].U16[0], state.v[22].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[17].U16[1], state.v[22].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[17].U16[2], state.v[22].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[17].U16[3], state.v[22].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[17].U16[4], state.v[22].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[17].U16[5], state.v[22].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[17].U16[6], state.v[22].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[17].U16[7], state.v[22].U16[7], s2value, 1, 0, 0, 1)
	}


_040018D0:	// 40018d0: <4b7c5f73>	VMOV      vec29[3] = vec28[3]
	state.v[29].U16[4] = state.v[28].U16[4];


_040018D4:	// 40018d4: <4a118c50>	VADD      vec17 = vec17 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[17].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[17].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[17].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[17].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[17].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[17].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[17].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[17].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018D8:	// 40018d8: <caaf2001>	LQV       vec15 <00> = [S5 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[15].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[15].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[15].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[15].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[15].U64[0] = state.v[15].U64[0] & ~temp1;
			state.v[15].U64[1] = state.v[15].U64[1] & ~temp2;
			state.v[15].U64[0] = state.v[15].U64[0] | value1;
			state.v[15].U64[1] = state.v[15].U64[1] | value2;
		}
	}


_040018DC:	// 40018dc: <4b12f780>	VMULF     vec30 = ( acc = (vec30 * vec18[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMULF_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 0, 0, 1)
	}


_040018E0:	// 40018e0: <4a11df10>	VADD      vec28 = vec27 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[17].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[17].U16[2], 0, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[17].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[17].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[17].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[17].U16[7], 0, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018E4:	// 40018e4: <4bbf8ed0>	VADD      vec27 = vec17 + vec31[5]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[2];
		VADD_operation(0, state.v[27].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018E8:	// 40018e8: <4b9cac00>	VMULF     vec16 = ( acc = (vec21 * vec28[4]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[3];
		VMULF_operation(0, state.v[16].U16[0], state.v[21].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[21].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[21].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[21].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[21].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[21].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[21].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[21].U16[7], s2value, 1, 0, 0, 1)
	}


_040018EC:	// 40018ec: <4b9c6773>	VMOV      vec29[4] = vec28[4]
	state.v[29].U16[3] = state.v[28].U16[3];


_040018F0:	// 40018f0: <4a108410>	VADD      vec16 = vec16 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018F4:	// 40018f4: <caaf2802>	LRV       vec15 <00> = [S5 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S5]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[15].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040018F8:	// 40018f8: <4a1e5ad0>	VADD      vec11 = vec11 + vec30[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[30].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[30].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[30].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[30].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[30].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[30].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[30].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[30].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018FC:	// 40018fc: <4a10df10>	VADD      vec28 = vec27 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[16].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[16].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[16].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[16].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[16].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[16].U16[7], 0, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001900:	// 4001900: <4bdf86d0>	VADD      vec27 = vec16 + vec31[6]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[1];
		VADD_operation(0, state.v[27].U16[0], state.v[16].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[16].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[16].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[16].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[16].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[16].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[16].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[16].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001904:	// 4001904: <4bbca440>	VMULF     vec17 = ( acc = (vec20 * vec28[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[2];
		VMULF_operation(0, state.v[17].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[17].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[17].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[17].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[17].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[17].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[17].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[17].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
	}


_04001908:	// 4001908: <4bbc6f73>	VMOV      vec29[5] = vec28[5]
	state.v[29].U16[2] = state.v[28].U16[2];


_0400190C:	// 400190c: <4b127b40>	VMULF     vec13 = ( acc = (vec15 * vec18[0]) << 1 ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMULF_operation(0, state.v[13].U16[0], state.v[15].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[13].U16[1], state.v[15].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[13].U16[2], state.v[15].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[13].U16[3], state.v[15].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[13].U16[4], state.v[15].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[13].U16[5], state.v[15].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[13].U16[6], state.v[15].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[13].U16[7], state.v[15].U16[7], s2value, 1, 0, 0, 1)
	}


_04001910:	// 4001910: <4a118c50>	VADD      vec17 = vec17 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[17].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[17].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[17].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[17].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[17].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[17].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[17].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[17].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001914:	// 4001914: <4a11df10>	VADD      vec28 = vec27 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[17].U16[2], 0, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[17].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[17].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[17].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[17].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[17].U16[7], 0, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001918:	// 4001918: <4bff8ed0>	VADD      vec27 = vec17 + vec31[7]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[31].U16[0];
		VADD_operation(0, state.v[27].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[27].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[27].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[27].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[27].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[27].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[27].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[27].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400191C:	// 400191c: <4bdc9c00>	VMULF     vec16 = ( acc = (vec19 * vec28[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[28].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[19].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[19].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[19].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[19].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[19].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[19].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[19].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[19].U16[7], s2value, 1, 1, 1, 1)
	}


_04001920:	// 4001920: <22b50010>	ADDI      S5 = S5 + 0010
	state.r[S5] = state.r[S5] + 0x0010;


_04001924:	// 4001924: <4bdc7773>	VMOV      vec29[6] = vec28[6]
	state.v[29].U16[1] = state.v[28].U16[1];


_04001928:	// 4001928: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_0400192C:	// 400192c: <4a0d5fd0>	VADD      vec31 = vec11 + vec13[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[31].U16[0], state.v[11].U16[0], state.v[13].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[31].U16[1], state.v[11].U16[1], state.v[13].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[31].U16[2], state.v[11].U16[2], state.v[13].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[31].U16[3], state.v[11].U16[3], state.v[13].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[31].U16[4], state.v[11].U16[4], state.v[13].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[31].U16[5], state.v[11].U16[5], state.v[13].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[31].U16[6], state.v[11].U16[6], state.v[13].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[31].U16[7], state.v[11].U16[7], state.v[13].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001930:	// 4001930: <4a108410>	VADD      vec16 = vec16 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001934:	// 4001934: <4a10df10>	VADD      vec28 = vec27 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[27].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[27].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[27].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[27].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[27].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[27].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[27].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[27].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001938:	// 4001938: <1ec0ffc6>	BGTZ      (R0>0) --> 1854
	sp_reg_pc = 0x04001938;
	if((_s32)state.r[S6] > 0)
	{
		state.r[S7] = state.r[S7] + 0x0010;
		goto _04001854;
	}


_0400193C:	// 400193c: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_04001940:	// 4001940: <4bfc7f73>	VMOV      vec29[7] = vec28[7]
	state.v[29].U16[0] = state.v[28].U16[0];


_04001944:	// 4001944: <4a1debaa>	VOR       vec14 = vec29 or vec29[<none>]
	rsp_cop2_vor(&state.v[14], &state.v[29], &state.v[29]);


_04001948:	// 4001948: <e82f1000>	SLV       [AT + 0x00] = vec15 <00>
	Save32_DMEM (state.v[15].U32[3], 0x00000000 + state.r[AT]);


_0400194C:	// 400194c: <e83d1601>	SLV       [AT + 0x08] = vec29 <12>
	Save32_DMEM (state.v[29].U32[0], 0x00000004 + state.r[AT]);


_04001950:	// 4001950: <0d000466>	JAL	    04001198
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}
	sp_reg_pc = 0x04001950;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001954:	// 4001954: <eaee207f>	SQV       [S7 + 0xf0] = vec14 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[14].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[14].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[14].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[14].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[14].U64[0] & temp1);
			value2 |= (state.v[14].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001958:	// 4001958: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_0400195C:	// 400195c: <14a0ffff>	BNE       (R0!=A1) --> 195c
	sp_reg_pc = 0x0400195C;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _0400195C;
	}


_04001960:	// 4001960: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001964:	// 4001964: <09000443>	J         0400110c
	goto _0400110C;


_04001968:	// 4001968: <00000000>	NOP       


_0400196C:	// 400196c: <00199e02>	SRL       S3 = T9 >> 24
	state.r[S3] = ((_u32)state.r[T9] >> 24);


_04001970:	// 4001970: <20140400>	ADDI      S4 = R0 + 0400
	state.r[S4] = 0 + 0x0400;


_04001974:	// 4001974: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001978:	// 4001978: <12600002>	BEQ       (R0==S3) --> 1984
	sp_reg_pc = 0x04001978;
	if (state.r[S3] == 0)
	{
		state.r[S7] = 0 + 0x0500;
		goto _04001984;
	}


_0400197C:	// 400197c: <20170500>	ADDI      S7 = R0 + 0500
	state.r[S7] = 0 + 0x0500;


_04001980:	// 4001980: <20170670>	ADDI      S7 = R0 + 0670
	state.r[S7] = 0 + 0x0670;


_04001984:	// 4001984: <ca9c2001>	LQV       vec28 <00> = [S4 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001988:	// 4001988: <4a16b5ac>	VXOR      vec22 = vec22 xor vec22[<none>]
	rsp_cop2_vxor(&state.v[22], &state.v[22], &state.v[22]);


_0400198C:	// 400198c: <489a9500>	MTC2      vec18 <10> = K0
	state.v[18].U16[2] = (_u16)state.r[K0];


_04001990:	// 4001990: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_04001994:	// 4001994: <001ad080>	SLL       K0 = K0 << 2
	state.r[K0] = ((_u32)state.r[K0] << 2);


_04001998:	// 4001998: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_0400199C:	// 400199c: <489aa000>	MTC2      vec20 <00> = K0
	state.v[20].U16[7] = (_u16)state.r[K0];


_040019A0:	// 40019a0: <4a19ce6c>	VXOR      vec25 = vec25 xor vec25[<none>]
	rsp_cop2_vxor(&state.v[25], &state.v[25], &state.v[25]);


_040019A4:	// 40019a4: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_040019A8:	// 40019a8: <4a1ad6ac>	VXOR      vec26 = vec26 xor vec26[<none>]
	rsp_cop2_vxor(&state.v[26], &state.v[26], &state.v[26]);


_040019AC:	// 40019ac: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_040019B0:	// 40019b0: <4a1bdeec>	VXOR      vec27 = vec27 xor vec27[<none>]
	rsp_cop2_vxor(&state.v[27], &state.v[27], &state.v[27]);


_040019B4:	// 40019b4: <20030007>	ADDI      V1 = R0 + 0007
	state.r[V1] = 0 + 0x0007;


_040019B8:	// 40019b8: <20130004>	ADDI      S3 = R0 + 0004
	state.r[S3] = 0 + 0x0004;


_040019BC:	// 40019bc: <48939000>	MTC2      vec18 <00> = S3
	state.v[18].U16[7] = (_u16)state.r[S3];


_040019C0:	// 40019c0: <20160170>	ADDI      S6 = R0 + 0170
	state.r[S6] = 0 + 0x0170;


_040019C4:	// 40019c4: <4b14e505>	VMUDM     vec20 = ( acc = vec28 * vec20[0] ) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMUDM_operation(0, state.v[20].U16[0], state.v[28].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[20].U16[1], state.v[28].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[20].U16[2], state.v[28].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[20].U16[3], state.v[28].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[20].U16[4], state.v[28].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[20].U16[5], state.v[28].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[20].U16[6], state.v[28].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[20].U16[7], state.v[28].U16[7], s2value, 1, 1, 1, 1)
	}


_040019C8:	// 40019c8: <001a9c02>	SRL       S3 = K0 >> 16
	state.r[S3] = ((_u32)state.r[K0] >> 16);


_040019CC:	// 40019cc: <32730001>	ANDI      S3 = S3 & 0001
	state.r[S3] = (state.r[S3] & 0x00000001);


_040019D0:	// 40019d0: <1e60001b>	BGTZ      (R0>0) --> 1a40
	sp_reg_pc = 0x040019D0;
	if((_s32)state.r[S3] > 0)
	{
		{
			_u32    addr = (0x00000010 + state.r[S4]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[20].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[20].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[20].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[20].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[20].U64[0] & temp1);
				value2 |= (state.v[20].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001A40;
	}


_040019D4:	// 40019d4: <ea942001>	SQV       [S4 + 0x10] = vec20 <00>
	{
		_u32    addr = (0x00000010 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[20].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[20].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[20].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[20].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[20].U64[0] & temp1);
			value2 |= (state.v[20].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019D8:	// 40019d8: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_040019DC:	// 40019dc: <40132800>	MFC0      A1 = unknown
	state.r[S3] = 0;


_040019E0:	// 40019e0: <1660ffff>	BNE       (R0!=S3) --> 19e0
	sp_reg_pc = 0x040019E0;
	if (state.r[S3] != 0)
	{
		state.r[S3] = 0;
		goto _040019E0;
	}


_040019E4:	// 40019e4: <40132800>	MFC0      A1 = unknown
	state.r[S3] = 0;


_040019E8:	// 40019e8: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040019EC:	// 40019ec: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040019F0:	// 40019f0: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040019F0;
	DMARead(state.r[V1]);


_040019F4:	// 40019f4: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_040019F8:	// 40019f8: <ca9b2802>	LRV       vec27 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[27].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040019FC:	// 40019fc: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A00:	// 4001a00: <ca9a2802>	LRV       vec26 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[26].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A04:	// 4001a04: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A08:	// 4001a08: <ca992802>	LRV       vec25 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[25].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A0C:	// 4001a0c: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A10:	// 4001a10: <ca982802>	LRV       vec24 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[24].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A14:	// 4001a14: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A18:	// 4001a18: <ca972802>	LRV       vec23 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[23].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A1C:	// 4001a1c: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A20:	// 4001a20: <ca962802>	LRV       vec22 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[22].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A24:	// 4001a24: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A28:	// 4001a28: <ca952802>	LRV       vec21 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[21].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A2C:	// 4001a2c: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A30:	// 4001a30: <14a0ffff>	BNE       (R0!=A1) --> 1a30
	sp_reg_pc = 0x04001A30;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001A30;
	}


_04001A34:	// 4001a34: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001A38:	// 4001a38: <0900069f>	J         04001a7c
	rsp_ldv_compiler((state.r[AT] & 0xFFF), 30, 0);
	goto _04001A7C;


_04001A3C:	// 4001a3c: <c83e1c00>	LDV       vec30 <08> = [AT + 0x00]
	rsp_ldv_compiler((state.r[AT] & 0xFFF), 30, 0);


_04001A40:	// 4001a40: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A44:	// 4001a44: <4a1ef7ac>	VXOR      vec30 = vec30 xor vec30[<none>]
	rsp_cop2_vxor(&state.v[30], &state.v[30], &state.v[30]);


_04001A48:	// 4001a48: <ca9b2802>	LRV       vec27 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[27].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A4C:	// 4001a4c: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A50:	// 4001a50: <ca9a2802>	LRV       vec26 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[26].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A54:	// 4001a54: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A58:	// 4001a58: <ca992802>	LRV       vec25 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[25].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A5C:	// 4001a5c: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A60:	// 4001a60: <ca982802>	LRV       vec24 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[24].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A64:	// 4001a64: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A68:	// 4001a68: <ca972802>	LRV       vec23 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[23].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A6C:	// 4001a6c: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A70:	// 4001a70: <ca962802>	LRV       vec22 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[22].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A74:	// 4001a74: <2294fffe>	ADDI      S4 = S4 + fffe
	state.r[S4] = state.r[S4] + 0xFFFFFFFE;


_04001A78:	// 4001a78: <ca952802>	LRV       vec21 <00> = [S4 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[S4]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[21].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001A7C:	// 4001a7c: <caff2000>	LQV       vec31 <00> = [S7 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001A80:	// 4001a80: <4bfee507>	VMUDH     vec20 = ( acc = (vec28 * vec30[7]) << 16) >> 16
	{
		_u16		s2value = state.v[30].U16[0];
		VMUDH_operation(0, state.v[20].U16[0], state.v[28].U16[0], s2value, 0, 1, 0, 1)
		VMUDH_operation(1, state.v[20].U16[1], state.v[28].U16[1], s2value, 0, 1, 0, 1)
		VMUDH_operation(2, state.v[20].U16[2], state.v[28].U16[2], s2value, 0, 1, 0, 1)
		VMUDH_operation(3, state.v[20].U16[3], state.v[28].U16[3], s2value, 0, 1, 0, 1)
		VMUDH_operation(4, state.v[20].U16[4], state.v[28].U16[4], s2value, 0, 1, 0, 1)
		VMUDH_operation(5, state.v[20].U16[5], state.v[28].U16[5], s2value, 0, 1, 0, 1)
		VMUDH_operation(6, state.v[20].U16[6], state.v[28].U16[6], s2value, 0, 1, 0, 1)
		VMUDH_operation(7, state.v[20].U16[7], state.v[28].U16[7], s2value, 0, 1, 0, 1)
	}


_04001A84:	// 4001a84: <4b1fdd0f>	VMADH     vec20 = ( acc+= (vec27 * vec31[0]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[7];
		VMADH_operation(0, state.v[20].U16[0], state.v[27].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[27].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[27].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[27].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[27].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[27].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[27].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[27].U16[7], s2value, 0, 1, 0, 1)
	}


_04001A88:	// 4001a88: <22d6fff0>	ADDI      S6 = S6 + fff0
	state.r[S6] = state.r[S6] + 0xFFFFFFF0;


_04001A8C:	// 4001a8c: <4b3fd50f>	VMADH     vec20 = ( acc+= (vec26 * vec31[1]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[6];
		VMADH_operation(0, state.v[20].U16[0], state.v[26].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[26].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[26].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[26].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[26].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[26].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[26].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[26].U16[7], s2value, 0, 1, 0, 1)
	}


_04001A90:	// 4001a90: <4b5fcd0f>	VMADH     vec20 = ( acc+= (vec25 * vec31[2]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[5];
		VMADH_operation(0, state.v[20].U16[0], state.v[25].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[25].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[25].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[25].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[25].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[25].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[25].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[25].U16[7], s2value, 0, 1, 0, 1)
	}


_04001A94:	// 4001a94: <eafe207f>	SQV       [S7 + 0xf0] = vec30 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A98:	// 4001a98: <4b7fc50f>	VMADH     vec20 = ( acc+= (vec24 * vec31[3]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[4];
		VMADH_operation(0, state.v[20].U16[0], state.v[24].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[24].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[24].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[24].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[24].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[24].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[24].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[24].U16[7], s2value, 0, 1, 0, 1)
	}


_04001A9C:	// 4001a9c: <4b9fbf8f>	VMADH     vec30 = ( acc+= (vec23 * vec31[4]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[3];
		VMADH_operation(0, state.v[30].U16[0], state.v[23].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[30].U16[1], state.v[23].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[30].U16[2], state.v[23].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[30].U16[3], state.v[23].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[30].U16[4], state.v[23].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[30].U16[5], state.v[23].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[30].U16[6], state.v[23].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[30].U16[7], state.v[23].U16[7], s2value, 0, 1, 0, 1)
	}


_04001AA0:	// 4001aa0: <4bbfb50f>	VMADH     vec20 = ( acc+= (vec22 * vec31[5]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[2];
		VMADH_operation(0, state.v[20].U16[0], state.v[22].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[22].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[22].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[22].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[22].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[22].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[22].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[22].U16[7], s2value, 0, 1, 0, 1)
	}


_04001AA4:	// 4001aa4: <4bdfad0f>	VMADH     vec20 = ( acc+= (vec21 * vec31[6]) << 16) >> 16
	{
		_u16		s2value = state.v[31].U16[1];
		VMADH_operation(0, state.v[20].U16[0], state.v[21].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[21].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[21].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[21].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[21].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[21].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[21].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[21].U16[7], s2value, 0, 1, 0, 1)
	}


_04001AA8:	// 4001aa8: <4bb2fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec18[5]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[2];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 0, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 0, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 0, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 0, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 0, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 0, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 0, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 0, 1, 0, 1)
	}


_04001AAC:	// 4001aac: <caff2001>	LQV       vec31 <00> = [S7 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[S7]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001AB0:	// 4001ab0: <4b3e9f5d>	VSAW $v29, $v19, $v30[[1]]
	state.v[29].U16[0] = accumulator[0];
	state.v[29].U16[1] = accumulator[1];
	state.v[29].U16[2] = accumulator[2];
	state.v[29].U16[3] = accumulator[3];
	state.v[29].U16[4] = accumulator[4];
	state.v[29].U16[5] = accumulator[5];
	state.v[29].U16[6] = accumulator[6];
	state.v[29].U16[7] = accumulator[7];


_04001AB4:	// 4001ab4: <4b1e9f9d>	VSAW $v30, $v19, $v30[[0]]
	state.v[30].U16[0] = accumulator_hi[0].U16[1];
	state.v[30].U16[1] = accumulator_hi[1].U16[1];
	state.v[30].U16[2] = accumulator_hi[2].U16[1];
	state.v[30].U16[3] = accumulator_hi[3].U16[1];
	state.v[30].U16[4] = accumulator_hi[4].U16[1];
	state.v[30].U16[5] = accumulator_hi[5].U16[1];
	state.v[30].U16[6] = accumulator_hi[6].U16[1];
	state.v[30].U16[7] = accumulator_hi[7].U16[1];


_04001AB8:	// 4001ab8: <4b12ed06>	VMUDN     vec20 = ( acc = vec29 * vec18[0]      ) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMUDN_operation(0, state.v[20].U16[0], state.v[29].U16[0], s2value, 1, 1, 1, 1)
		VMUDN_operation(1, state.v[20].U16[1], state.v[29].U16[1], s2value, 1, 1, 1, 1)
		VMUDN_operation(2, state.v[20].U16[2], state.v[29].U16[2], s2value, 1, 1, 1, 1)
		VMUDN_operation(3, state.v[20].U16[3], state.v[29].U16[3], s2value, 1, 1, 1, 1)
		VMUDN_operation(4, state.v[20].U16[4], state.v[29].U16[4], s2value, 1, 1, 1, 1)
		VMUDN_operation(5, state.v[20].U16[5], state.v[29].U16[5], s2value, 1, 1, 1, 1)
		VMUDN_operation(6, state.v[20].U16[6], state.v[29].U16[6], s2value, 1, 1, 1, 1)
		VMUDN_operation(7, state.v[20].U16[7], state.v[29].U16[7], s2value, 1, 1, 1, 1)
	}


_04001ABC:	// 4001abc: <4b12f78f>	VMADH     vec30 = ( acc+= (vec30 * vec18[0]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[7];
		VMADH_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 1, 0, 1)
	}


_04001AC0:	// 4001ac0: <1ec0ffef>	BGTZ      (R0>0) --> 1a80
	sp_reg_pc = 0x04001AC0;
	if((_s32)state.r[S6] > 0)
	{
		state.r[S7] = state.r[S7] + 0x0010;
		goto _04001A80;
	}


_04001AC4:	// 4001ac4: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_04001AC8:	// 4001ac8: <22e1fff8>	ADDI      AT = S7 + fff8
	state.r[AT] = state.r[S7] + 0xFFFFFFF8;


_04001ACC:	// 4001acc: <0d000466>	JAL	    04001198
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}
	sp_reg_pc = 0x04001ACC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001AD0:	// 4001ad0: <eafe207f>	SQV       [S7 + 0xf0] = vec30 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[30].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[30].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[30].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[30].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[30].U64[0] & temp1);
			value2 |= (state.v[30].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001AD4:	// 4001ad4: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001AD8:	// 4001ad8: <14a0ffff>	BNE       (R0!=A1) --> 1ad8
	sp_reg_pc = 0x04001AD8;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001AD8;
	}


_04001ADC:	// 4001adc: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001AE0:	// 4001ae0: <09000443>	J         0400110c
	goto _0400110C;


_04001AE4:	// 4001ae4: <00000000>	NOP       


_04001AE8:	// 4001ae8: <20020170>	ADDI      V0 = R0 + 0170
	state.r[V0] = 0 + 0x0170;


_04001AEC:	// 4001aec: <23410500>	ADDI      AT = K0 + 0500
	state.r[AT] = state.r[K0] + 0x0500;


_04001AF0:	// 4001af0: <48990800>	MTC2      vec1 <00> = T9
	state.v[1].U16[7] = (_u16)state.r[T9];


_04001AF4:	// 4001af4: <00191c02>	SRL       V1 = T9 >> 16
	state.r[V1] = ((_u32)state.r[T9] >> 16);


_04001AF8:	// 4001af8: <48830900>	MTC2      vec1 <02> = V1
	state.v[1].U16[6] = (_u16)state.r[V1];


_04001AFC:	// 4001afc: <c8222000>	LQV       vec02 <00> = [AT + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[AT]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_04001B00:	// 4001b00: <2042fff0>	ADDI      V0 = V0 + fff0
	state.r[V0] = state.r[V0] + 0xFFFFFFF0;


_04001B04:	// 4001b04: <20210010>	ADDI      AT = AT + 0010
	state.r[AT] = state.r[AT] + 0x0010;


_04001B08:	// 4001b08: <4b0110c7>	VMUDH     vec03 = ( acc = (vec02 * vec01[0]) << 16) >> 16
	{
		_u16		s2value = state.v[1].U16[7];
		VMUDH_operation(0, state.v[3].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VMUDH_operation(1, state.v[3].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VMUDH_operation(2, state.v[3].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VMUDH_operation(3, state.v[3].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		VMUDH_operation(4, state.v[3].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VMUDH_operation(5, state.v[3].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VMUDH_operation(6, state.v[3].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VMUDH_operation(7, state.v[3].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B0C:	// 4001b0c: <4b2118c5>	VMUDM     vec03 = ( acc = vec03 * vec01[1] ) >> 16
	{
		_u16		s2value = state.v[1].U16[6];
		VMUDM_operation(0, state.v[3].U16[0], state.v[3].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[3].U16[1], state.v[3].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[3].U16[2], state.v[3].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[3].U16[3], state.v[3].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[3].U16[4], state.v[3].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[3].U16[5], state.v[3].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[3].U16[6], state.v[3].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[3].U16[7], state.v[3].U16[7], s2value, 1, 1, 1, 1)
	}


_04001B10:	// 4001b10: <1c40fffa>	BGTZ      (R0>0) --> 1afc
	sp_reg_pc = 0x04001B10;
	if((_s32)state.r[V0] > 0)
	{
		{
			_u32    addr = (0xFFFFFFF0 + state.r[AT]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[3].U64[0] & temp1);
				value2 |= (state.v[3].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001AFC;
	}


_04001B14:	// 4001b14: <e823207f>	SQV       [AT + 0xf0] = vec03 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[AT]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[3].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[3].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[3].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[3].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[3].U64[0] & temp1);
			value2 |= (state.v[3].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001B18:	// 4001b18: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001B1C:	// 4001b1c: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001B20:	// 4001b20: <001a1200>	SLL       V0 = K0 << 8
	state.r[V0] = ((_u32)state.r[K0] << 8);


_04001B24:	// 4001b24: <4a17bdec>	VXOR      vec23 = vec23 xor vec23[<none>]
	rsp_cop2_vxor(&state.v[23], &state.v[23], &state.v[23]);


_04001B28:	// 4001b28: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001B2C:	// 4001b2c: <2003000f>	ADDI      V1 = R0 + 000f
	state.r[V1] = 0 + 0x000F;


_04001B30:	// 4001b30: <0019af82>	SRL       S5 = T9 >> 30
	state.r[S5] = ((_u32)state.r[T9] >> 30);


_04001B34:	// 4001b34: <1ea00012>	BGTZ      (R0>0) --> 1b80
	sp_reg_pc = 0x04001B34;
	if((_s32)state.r[S5] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001B80;
	}


_04001B38:	// 4001b38: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_04001B3C:	// 4001b3c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001B40:	// 4001b40: <1480ffff>	BNE       (R0!=A0) --> 1b40
	sp_reg_pc = 0x04001B40;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001B40;
	}


_04001B44:	// 4001b44: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001B48:	// 4001b48: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_04001B4C:	// 4001b4c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001B50:	// 4001b50: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001B50;
	DMARead(state.r[V1]);


_04001B54:	// 4001b54: <0019a082>	SRL       S4 = T9 >> 2
	state.r[S4] = ((_u32)state.r[T9] >> 2);


_04001B58:	// 4001b58: <32940fff>	ANDI      S4 = S4 & 0fff
	state.r[S4] = (state.r[S4] & 0x00000FFF);


_04001B5C:	// 4001b5c: <229404f8>	ADDI      S4 = S4 + 04f8
	state.r[S4] = state.r[S4] + 0x04F8;


_04001B60:	// 4001b60: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001B64:	// 4001b64: <c8192004>	LQV       vec25 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_04001B68:	// 4001b68: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001B6C:	// 4001b6c: <14a0ffff>	BNE       (R0!=A1) --> 1b6c
	sp_reg_pc = 0x04001B6C;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001B6C;
	}


_04001B70:	// 4001b70: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001B74:	// 4001b74: <cb131800>	LDV       vec19 <00> = [T8 + 0x00]
	rsp_ldv_compiler((state.r[T8] & 0xFFF), 19, 8);


_04001B78:	// 4001b78: <090006e7>	J         04001b9c
	state.v[24].U16[0] = Load16_DMEM(0x00000008 + state.r[T8]);
	goto _04001B9C;


_04001B7C:	// 4001b7c: <cb180f04>	LSV       vec24 <14> = [T8 + 0x08]
	state.v[24].U16[0] = Load16_DMEM(0x00000008 + state.r[T8]);


_04001B80:	// 4001b80: <0019a082>	SRL       S4 = T9 >> 2
	state.r[S4] = ((_u32)state.r[T9] >> 2);


_04001B84:	// 4001b84: <32940fff>	ANDI      S4 = S4 & 0fff
	state.r[S4] = (state.r[S4] & 0x00000FFF);


_04001B88:	// 4001b88: <229404f8>	ADDI      S4 = S4 + 04f8
	state.r[S4] = state.r[S4] + 0x04F8;


_04001B8C:	// 4001b8c: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001B90:	// 4001b90: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001B94:	// 4001b94: <c8192004>	LQV       vec25 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[25].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[25].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[25].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[25].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[25].U64[0] = state.v[25].U64[0] & ~temp1;
			state.v[25].U64[1] = state.v[25].U64[1] & ~temp2;
			state.v[25].U64[0] = state.v[25].U64[0] | value1;
			state.v[25].U64[1] = state.v[25].U64[1] | value2;
		}
	}


_04001B98:	// 4001b98: <4a18c62c>	VXOR      vec24 = vec24 xor vec24[<none>]
	rsp_cop2_vxor(&state.v[24], &state.v[24], &state.v[24]);


_04001B9C:	// 4001b9c: <4894aa00>	MTC2      vec21 <04> = S4
	// In function: Tooie1B9C_1C58
	//Call the function: Tooie1B9C_1C58
	Tooie1B9C_1C58();
	goto _04001C5C;

_04001BA0:	// 4001ba0: <200400c0>	ADDI      A0 = R0 + 00c0
	ErrorToCallFunction(0x04001BA0);

_04001BA4:	// 4001ba4: <4884ab00>	MTC2      vec21 <06> = A0
	ErrorToCallFunction(0x04001BA4);

_04001BA8:	// 4001ba8: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
	ErrorToCallFunction(0x04001BA8);

_04001BAC:	// 4001bac: <00192382>	SRL       A0 = T9 >> 14
	ErrorToCallFunction(0x04001BAC);

_04001BB0:	// 4001bb0: <4884ac00>	MTC2      vec21 <08> = A0
	ErrorToCallFunction(0x04001BB0);

_04001BB4:	// 4001bb4: <20040040>	ADDI      A0 = R0 + 0040
	ErrorToCallFunction(0x04001BB4);

_04001BB8:	// 4001bb8: <4884ad00>	MTC2      vec21 <10> = A0
	ErrorToCallFunction(0x04001BB8);

_04001BBC:	// 4001bbc: <4a1fce51>	VSUB      vec25 = vec25 - vec31[<none>]
	ErrorToCallFunction(0x04001BBC);

_04001BC0:	// 4001bc0: <c81e2006>	LQV       vec30 <00> = [R0 + 0x60]
	ErrorToCallFunction(0x04001BC0);

_04001BC4:	// 4001bc4: <c81d2007>	LQV       vec29 <00> = [R0 + 0x70]
	ErrorToCallFunction(0x04001BC4);

_04001BC8:	// 4001bc8: <c81c2008>	LQV       vec28 <00> = [R0 + 0x80]
	ErrorToCallFunction(0x04001BC8);

_04001BCC:	// 4001bcc: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	ErrorToCallFunction(0x04001BCC);

_04001BD0:	// 4001bd0: <c81b2009>	LQV       vec27 <00> = [R0 + 0x90]
	ErrorToCallFunction(0x04001BD0);

_04001BD4:	// 4001bd4: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	ErrorToCallFunction(0x04001BD4);

_04001BD8:	// 4001bd8: <c81a200a>	LQV       vec26 <00> = [R0 + 0xa0]
	ErrorToCallFunction(0x04001BD8);

_04001BDC:	// 4001bdc: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x04001BDC);

_04001BE0:	// 4001be0: <ea931800>	SDV       [S4 + 0x00] = vec19 <00>
	ErrorToCallFunction(0x04001BE0);

_04001BE4:	// 4001be4: <c8192004>	LQV       vec25 <00> = [R0 + 0x40]
	ErrorToCallFunction(0x04001BE4);

_04001BE8:	// 4001be8: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	ErrorToCallFunction(0x04001BE8);

_04001BEC:	// 4001bec: <20160170>	ADDI      S6 = R0 + 0170
	ErrorToCallFunction(0x04001BEC);

_04001BF0:	// 4001bf0: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	ErrorToCallFunction(0x04001BF0);

_04001BF4:	// 4001bf4: <33240003>	ANDI      A0 = T9 & 0003
	ErrorToCallFunction(0x04001BF4);

_04001BF8:	// 4001bf8: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	ErrorToCallFunction(0x04001BF8);

_04001BFC:	// 4001bfc: <10800002>	BEQ       (R0==A0) --> 1c08
	ErrorToCallFunction(0x04001BFC);

_04001C00:	// 4001c00: <20170500>	ADDI      S7 = R0 + 0500
	ErrorToCallFunction(0x04001C00);

_04001C04:	// 4001c04: <20170670>	ADDI      S7 = R0 + 0670
	ErrorToCallFunction(0x04001C04);

_04001C08:	// 4001c08: <eb180b84>	SSV       [T8 + 0x08] = vec24 <07>
	ErrorToCallFunction(0x04001C08);

_04001C0C:	// 4001c0c: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x04001C0C);

_04001C10:	// 4001c10: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	ErrorToCallFunction(0x04001C10);

_04001C14:	// 4001c14: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	ErrorToCallFunction(0x04001C14);

_04001C18:	// 4001c18: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	ErrorToCallFunction(0x04001C18);

_04001C1C:	// 4001c1c: <84150fb0>	LH        S5 = [R0+0fb0]
	ErrorToCallFunction(0x04001C1C);

_04001C20:	// 4001c20: <840d0fc0>	LH        T5 = [R0+0fc0]
	ErrorToCallFunction(0x04001C20);

_04001C24:	// 4001c24: <84110fb8>	LH        S1 = [R0+0fb8]
	ErrorToCallFunction(0x04001C24);

_04001C28:	// 4001c28: <84090fc8>	LH        T1 = [R0+0fc8]
	ErrorToCallFunction(0x04001C28);

_04001C2C:	// 4001c2c: <84140fb2>	LH        S4 = [R0+0fb2]
	ErrorToCallFunction(0x04001C2C);

_04001C30:	// 4001c30: <840c0fc2>	LH        T4 = [R0+0fc2]
	ErrorToCallFunction(0x04001C30);

_04001C34:	// 4001c34: <84100fba>	LH        S0 = [R0+0fba]
	ErrorToCallFunction(0x04001C34);

_04001C38:	// 4001c38: <84080fca>	LH        T0 = [R0+0fca]
	ErrorToCallFunction(0x04001C38);

_04001C3C:	// 4001c3c: <84130fb4>	LH        S3 = [R0+0fb4]
	ErrorToCallFunction(0x04001C3C);

_04001C40:	// 4001c40: <840b0fc4>	LH        T3 = [R0+0fc4]
	ErrorToCallFunction(0x04001C40);

_04001C44:	// 4001c44: <840f0fbc>	LH        T7 = [R0+0fbc]
	ErrorToCallFunction(0x04001C44);

_04001C48:	// 4001c48: <84070fcc>	LH        A3 = [R0+0fcc]
	ErrorToCallFunction(0x04001C48);

_04001C4C:	// 4001c4c: <84120fb6>	LH        S2 = [R0+0fb6]
	ErrorToCallFunction(0x04001C4C);

_04001C50:	// 4001c50: <840a0fc6>	LH        T2 = [R0+0fc6]
	ErrorToCallFunction(0x04001C50);

_04001C54:	// 4001c54: <840e0fbe>	LH        T6 = [R0+0fbe]
	ErrorToCallFunction(0x04001C54);

_04001C58:	// 4001c58: <84060fce>	LH        A2 = [R0+0fce]
	ErrorToCallFunction(0x04001C58);

_04001C5C:	// 4001c5c: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	// In function: SmashBros193C_1A28
	//Call the function: SmashBros193C_1A28
	SmashBros193C_1A28();
	goto _04001D4C;

_04001C60:	// 4001c60: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	ErrorToCallFunction(0x04001C60);

_04001C64:	// 4001c64: <c9b21800>	LDV       vec18 <00> = [T5 + 0x00]
	ErrorToCallFunction(0x04001C64);

_04001C68:	// 4001c68: <4bf7fe0f>	VMADH     vec24 = ( acc+= (vec31 * vec23[7]) << 16) >> 16
	ErrorToCallFunction(0x04001C68);

_04001C6C:	// 4001c6c: <ca331c00>	LDV       vec19 <08> = [S1 + 0x00]
	ErrorToCallFunction(0x04001C6C);

_04001C70:	// 4001c70: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	ErrorToCallFunction(0x04001C70);

_04001C74:	// 4001c74: <c9321c00>	LDV       vec18 <08> = [T1 + 0x00]
	ErrorToCallFunction(0x04001C74);

_04001C78:	// 4001c78: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x04001C78);

_04001C7C:	// 4001c7c: <ca911800>	LDV       vec17 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001C7C);

_04001C80:	// 4001c80: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	ErrorToCallFunction(0x04001C80);

_04001C84:	// 4001c84: <c9901800>	LDV       vec16 <00> = [T4 + 0x00]
	ErrorToCallFunction(0x04001C84);

_04001C88:	// 4001c88: <ca111c00>	LDV       vec17 <08> = [S0 + 0x00]
	ErrorToCallFunction(0x04001C88);

_04001C8C:	// 4001c8c: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	ErrorToCallFunction(0x04001C8C);

_04001C90:	// 4001c90: <c9101c00>	LDV       vec16 <08> = [T0 + 0x00]
	ErrorToCallFunction(0x04001C90);

_04001C94:	// 4001c94: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	ErrorToCallFunction(0x04001C94);

_04001C98:	// 4001c98: <ca6f1800>	LDV       vec15 <00> = [S3 + 0x00]
	ErrorToCallFunction(0x04001C98);

_04001C9C:	// 4001c9c: <c96e1800>	LDV       vec14 <00> = [T3 + 0x00]
	ErrorToCallFunction(0x04001C9C);

_04001CA0:	// 4001ca0: <c9ef1c00>	LDV       vec15 <08> = [T7 + 0x00]
	ErrorToCallFunction(0x04001CA0);

_04001CA4:	// 4001ca4: <c8ee1c00>	LDV       vec14 <08> = [A3 + 0x00]
	ErrorToCallFunction(0x04001CA4);

_04001CA8:	// 4001ca8: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x04001CA8);

_04001CAC:	// 4001cac: <ca4d1800>	LDV       vec13 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001CAC);

_04001CB0:	// 4001cb0: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	ErrorToCallFunction(0x04001CB0);

_04001CB4:	// 4001cb4: <c94c1800>	LDV       vec12 <00> = [T2 + 0x00]
	ErrorToCallFunction(0x04001CB4);

_04001CB8:	// 4001cb8: <c9cd1c00>	LDV       vec13 <08> = [T6 + 0x00]
	ErrorToCallFunction(0x04001CB8);

_04001CBC:	// 4001cbc: <4a129ac0>	VMULF     vec11 = ( acc = (vec19 * vec18[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CBC);

_04001CC0:	// 4001cc0: <c8cc1c00>	LDV       vec12 <08> = [A2 + 0x00]
	ErrorToCallFunction(0x04001CC0);

_04001CC4:	// 4001cc4: <4a108a80>	VMULF     vec10 = ( acc = (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CC4);

_04001CC8:	// 4001cc8: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	ErrorToCallFunction(0x04001CC8);

_04001CCC:	// 4001ccc: <4a0e7a40>	VMULF     vec09 = ( acc = (vec15 * vec14[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CCC);

_04001CD0:	// 4001cd0: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	ErrorToCallFunction(0x04001CD0);

_04001CD4:	// 4001cd4: <84150fb0>	LH        S5 = [R0+0fb0]
	ErrorToCallFunction(0x04001CD4);

_04001CD8:	// 4001cd8: <840d0fc0>	LH        T5 = [R0+0fc0]
	ErrorToCallFunction(0x04001CD8);

_04001CDC:	// 4001cdc: <4a0c6a00>	VMULF     vec08 = ( acc = (vec13 * vec12[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CDC);

_04001CE0:	// 4001ce0: <84110fb8>	LH        S1 = [R0+0fb8]
	ErrorToCallFunction(0x04001CE0);

_04001CE4:	// 4001ce4: <4a6b5ad0>	VADD      vec11 = vec11 + vec11[1q]
	ErrorToCallFunction(0x04001CE4);

_04001CE8:	// 4001ce8: <84090fc8>	LH        T1 = [R0+0fc8]
	ErrorToCallFunction(0x04001CE8);

_04001CEC:	// 4001cec: <4a6a5290>	VADD      vec10 = vec10 + vec10[1q]
	ErrorToCallFunction(0x04001CEC);

_04001CF0:	// 4001cf0: <84140fb2>	LH        S4 = [R0+0fb2]
	ErrorToCallFunction(0x04001CF0);

_04001CF4:	// 4001cf4: <4a694a50>	VADD      vec09 = vec09 + vec09[1q]
	ErrorToCallFunction(0x04001CF4);

_04001CF8:	// 4001cf8: <840c0fc2>	LH        T4 = [R0+0fc2]
	ErrorToCallFunction(0x04001CF8);

_04001CFC:	// 4001cfc: <4a684210>	VADD      vec08 = vec08 + vec08[1q]
	ErrorToCallFunction(0x04001CFC);

_04001D00:	// 4001d00: <84100fba>	LH        S0 = [R0+0fba]
	ErrorToCallFunction(0x04001D00);

_04001D04:	// 4001d04: <4acb5ad0>	VADD      vec11 = vec11 + vec11[2h]
	ErrorToCallFunction(0x04001D04);

_04001D08:	// 4001d08: <84080fca>	LH        T0 = [R0+0fca]
	ErrorToCallFunction(0x04001D08);

_04001D0C:	// 4001d0c: <4aca5290>	VADD      vec10 = vec10 + vec10[2h]
	ErrorToCallFunction(0x04001D0C);

_04001D10:	// 4001d10: <84130fb4>	LH        S3 = [R0+0fb4]
	ErrorToCallFunction(0x04001D10);

_04001D14:	// 4001d14: <4ac94a50>	VADD      vec09 = vec09 + vec09[2h]
	ErrorToCallFunction(0x04001D14);

_04001D18:	// 4001d18: <840b0fc4>	LH        T3 = [R0+0fc4]
	ErrorToCallFunction(0x04001D18);

_04001D1C:	// 4001d1c: <4ac84210>	VADD      vec08 = vec08 + vec08[2h]
	ErrorToCallFunction(0x04001D1C);

_04001D20:	// 4001d20: <840f0fbc>	LH        T7 = [R0+0fbc]
	ErrorToCallFunction(0x04001D20);

_04001D24:	// 4001d24: <4a8be9c6>	VMUDN     vec07 = ( acc = vec29 * vec11[0h]      ) >> 16
	ErrorToCallFunction(0x04001D24);

_04001D28:	// 4001d28: <84070fcc>	LH        A3 = [R0+0fcc]
	ErrorToCallFunction(0x04001D28);

_04001D2C:	// 4001d2c: <4a8ae1ce>	VMADN     vec07 = ( acc+= vec28 * vec10[0h] )
	ErrorToCallFunction(0x04001D2C);

_04001D30:	// 4001d30: <84120fb6>	LH        S2 = [R0+0fb6]
	ErrorToCallFunction(0x04001D30);

_04001D34:	// 4001d34: <4a89d9ce>	VMADN     vec07 = ( acc+= vec27 * vec09[0h] )
	ErrorToCallFunction(0x04001D34);

_04001D38:	// 4001d38: <840a0fc6>	LH        T2 = [R0+0fc6]
	ErrorToCallFunction(0x04001D38);

_04001D3C:	// 4001d3c: <4a88d1ce>	VMADN     vec07 = ( acc+= vec26 * vec08[0h] )
	ErrorToCallFunction(0x04001D3C);

_04001D40:	// 4001d40: <840e0fbe>	LH        T6 = [R0+0fbe]
	ErrorToCallFunction(0x04001D40);

_04001D44:	// 4001d44: <84060fce>	LH        A2 = [R0+0fce]
	ErrorToCallFunction(0x04001D44);

_04001D48:	// 4001d48: <22d6fff0>	ADDI      S6 = S6 + fff0
	ErrorToCallFunction(0x04001D48);

_04001D4C:	// 4001d4c: <1ac00003>	BLEZ      (R0<=0) --> 1d5c
	sp_reg_pc = 0x04001D4C;
	if ((_s32)state.r[S6] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S7]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[7].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[7].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[7].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[7].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[7].U64[0] & temp1);
				value2 |= (state.v[7].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001D5C;
	}


_04001D50:	// 4001d50: <eae72000>	SQV       [S7 + 0x00] = vec07 <00>
	{
		_u32    addr = (0x00000000 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[7].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[7].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[7].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[7].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[7].U64[0] & temp1);
			value2 |= (state.v[7].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001D54:	// 4001d54: <09000717>	J         04001c5c
	state.r[S7] = state.r[S7] + 0x0010;
	goto _04001C5C;


_04001D58:	// 4001d58: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_04001D5C:	// 4001d5c: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	rsp_ldv_compiler((state.r[S5] & 0xFFF), 19, 8);


_04001D60:	// 4001d60: <eb180804>	SSV       [T8 + 0x08] = vec24 <00>
	Save16_DMEM(state.v[24].U16[7], (0x00000008 + state.r[T8]) & 0xfff);


_04001D64:	// 4001d64: <0d000466>	JAL	    04001198
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}
	sp_reg_pc = 0x04001D64;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001D68:	// 4001d68: <eb131800>	SDV       [T8 + 0x00] = vec19 <00>
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}


_04001D6C:	// 4001d6c: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001D70:	// 4001d70: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001D74:	// 4001d74: <14a0ffff>	BNE       (R0!=A1) --> 1d74
	sp_reg_pc = 0x04001D74;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001D74;
	}


_04001D78:	// 4001d78: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001D7C:	// 4001d7c: <09000443>	J         0400110c
	goto _0400110C;


_04001D80:	// 4001d80: <00000000>	NOP       


_04001D84:	// 4001d84: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001D88:	// 4001d88: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001D8C:	// 4001d8c: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001D90:	// 4001d90: <c80a2006>	LQV       vec10 <00> = [R0 + 0x60]
	{
		_u32 addr = (0x00000060 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[10].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[10].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[10].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[10].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[10].U64[0] = state.v[10].U64[0] & ~temp1;
			state.v[10].U64[1] = state.v[10].U64[1] & ~temp2;
			state.v[10].U64[0] = state.v[10].U64[0] | value1;
			state.v[10].U64[1] = state.v[10].U64[1] | value2;
		}
	}


_04001D94:	// 4001d94: <c81e200b>	LQV       vec30 <00> = [R0 + 0xb0]
	{
		_u32 addr = (0x000000B0 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001D98:	// 4001d98: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001D9C:	// 4001d9c: <001a7402>	SRL       T6 = K0 >> 16
	state.r[T6] = ((_u32)state.r[K0] >> 16);


_04001DA0:	// 4001da0: <31cf0001>	ANDI      T7 = T6 & 0001
	state.r[T7] = (state.r[T6] & 0x00000001);


_04001DA4:	// 4001da4: <1de0000a>	BGTZ      (R0>0) --> 1dd0
	sp_reg_pc = 0x04001DA4;
	if((_s32)state.r[T7] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001DD0;
	}


_04001DA8:	// 4001da8: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_04001DAC:	// 4001dac: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x004F;
	sp_reg_pc = 0x04001DAC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001DB0:	// 4001db0: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001DB4:	// 4001db4: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001DB8:	// 4001db8: <14a0ffff>	BNE       (R0!=A1) --> 1db8
	sp_reg_pc = 0x04001DB8;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001DB8;
	}


_04001DBC:	// 4001dbc: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001DC0:	// 4001dc0: <cb142000>	LQV       vec20 <00> = [T8 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


_04001DC4:	// 4001dc4: <cb152001>	LQV       vec21 <00> = [T8 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


_04001DC8:	// 4001dc8: <cb122002>	LQV       vec18 <00> = [T8 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[18].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[18].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[18].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[18].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[18].U64[0] = state.v[18].U64[0] & ~temp1;
			state.v[18].U64[1] = state.v[18].U64[1] & ~temp2;
			state.v[18].U64[0] = state.v[18].U64[0] | value1;
			state.v[18].U64[1] = state.v[18].U64[1] | value2;
		}
	}


_04001DCC:	// 4001dcc: <cb132003>	LQV       vec19 <00> = [T8 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[19].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[19].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[19].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[19].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[19].U64[0] = state.v[19].U64[0] & ~temp1;
			state.v[19].U64[1] = state.v[19].U64[1] & ~temp2;
			state.v[19].U64[0] = state.v[19].U64[0] | value1;
			state.v[19].U64[1] = state.v[19].U64[1] | value2;
		}
	}


_04001DD0:	// 4001dd0: <cb182004>	LQV       vec24 <00> = [T8 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001DD4:	// 4001dd4: <20100500>	ADDI      S0 = R0 + 0500
	state.r[S0] = 0 + 0x0500;


_04001DD8:	// 4001dd8: <201509e0>	ADDI      S5 = R0 + 09e0
	state.r[S5] = 0 + 0x09E0;


_04001DDC:	// 4001ddc: <20140b50>	ADDI      S4 = R0 + 0b50
	state.r[S4] = 0 + 0x0B50;


_04001DE0:	// 4001de0: <20130cc0>	ADDI      S3 = R0 + 0cc0
	state.r[S3] = 0 + 0x0CC0;


_04001DE4:	// 4001de4: <20120e30>	ADDI      S2 = R0 + 0e30
	state.r[S2] = 0 + 0x0E30;


_04001DE8:	// 4001de8: <20110170>	ADDI      S1 = R0 + 0170
	state.r[S1] = 0 + 0x0170;


_04001DEC:	// 4001dec: <4816c400>	MFC2      S6 = vec24 <08>
	state.r[S6] = (_s32)state.v[24].S16[3];


_04001DF0:	// 4001df0: <4bd8fa68>	VAND      vec09 = vec31 and vec24[6]
	rsp_cop2_vand_element(&state.v[9], &state.v[31], &state.v[24].U16[1]);


_04001DF4:	// 4001df4: <4bf8fa28>	VAND      vec08 = vec31 and vec24[7]
	rsp_cop2_vand_element(&state.v[8], &state.v[31], &state.v[24].U16[0]);


_04001DF8:	// 4001df8: <4a090251>	VSUB      vec09 = vec00 - vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[0].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[0].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[0].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[0].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[0].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[0].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[0].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[0].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001DFC:	// 4001dfc: <4a080211>	VSUB      vec08 = vec00 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[8].U16[0], state.v[0].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[8].U16[1], state.v[0].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[8].U16[2], state.v[0].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[8].U16[3], state.v[0].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[8].U16[4], state.v[0].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[8].U16[5], state.v[0].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[8].U16[6], state.v[0].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[8].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E00:	// 4001e00: <4a09422c>	VXOR      vec08 = vec08 xor vec09[<none>]
	rsp_cop2_vxor(&state.v[8], &state.v[8], &state.v[9]);


_04001E04:	// 4001e04: <11e00035>	BEQ       (R0==T7) --> 1edc
	sp_reg_pc = 0x04001E04;
	if (state.r[T7] == 0)
	{
		state.r[S7] = (_s32)state.v[24].S16[6];
		goto _04001EDC;
	}


_04001E08:	// 4001e08: <4817c100>	MFC2      S7 = vec24 <02>
	state.r[S7] = (_s32)state.v[24].S16[6];


_04001E0C:	// 4001e0c: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001E10:	// 4001e10: <4a14a52c>	VXOR      vec20 = vec20 xor vec20[<none>]
	rsp_cop2_vxor(&state.v[20], &state.v[20], &state.v[20]);


_04001E14:	// 4001e14: <cb140f28>	LSV       vec20 <14> = [T8 + 0x10]
	state.v[20].U16[0] = Load16_DMEM(0x00000050 + state.r[T8]);


_04001E18:	// 4001e18: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001E1C:	// 4001e1c: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001E20:	// 4001e20: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_04001E24:	// 4001e24: <489a9700>	MTC2      vec18 <14> = K0
	state.v[18].U16[0] = (_u16)state.r[K0];


_04001E28:	// 4001e28: <4b58f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[2] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[5];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E2C:	// 4001e2c: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001E30:	// 4001e30: <4b38f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[1] )
	{
		_u16		s2value = state.v[24].U16[6];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E34:	// 4001e34: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001E38:	// 4001e38: <4bf4fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec20[7]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[0];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001E3C:	// 4001e3c: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001E40:	// 4001e40: <4b00fd4e>	VMADN     vec21 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[21].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[21].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[21].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[21].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[21].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[21].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[21].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[21].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E44:	// 4001e44: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001E44;
	if (state.r[S7] >= 0)
	{
		rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);
		goto _04001E54;
	}


_04001E48:	// 4001e48: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001E4C:	// 4001e4c: <09000796>	J         04001e58
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001E58;


_04001E50:	// 4001e50: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001E54:	// 4001e54: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001E58:	// 4001e58: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001E5C:	// 4001e5c: <4bb8f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[5] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[2];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E60:	// 4001e60: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001E64:	// 4001e64: <4b98f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[4] )
	{
		_u16		s2value = state.v[24].U16[3];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E68:	// 4001e68: <2231fff0>	ADDI      S1 = S1 + fff0
	state.r[S1] = state.r[S1] + 0xFFFFFFF0;


_04001E6C:	// 4001e6c: <4bf2fc8f>	VMADH     vec18 = ( acc+= (vec31 * vec18[7]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[0];
		VMADH_operation(0, state.v[18].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[18].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[18].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[18].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[18].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[18].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[18].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[18].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001E70:	// 4001e70: <22100010>	ADDI      S0 = S0 + 0010
	state.r[S0] = state.r[S0] + 0x0010;


_04001E74:	// 4001e74: <4b00fcce>	VMADN     vec19 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[19].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[19].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[19].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[19].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[19].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[19].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[19].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[19].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001E78:	// 4001e78: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E7C:	// 4001e7c: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001E7C;
	if (state.r[S6] >= 0)
	{
		{
			_u16		s2value = state.v[24].U16[0];
			VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
			VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
			VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
			VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
			VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
			VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
			VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
			VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
		}
		goto _04001E8C;
	}


_04001E80:	// 4001e80: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E84:	// 4001e84: <090007a4>	J         04001e90
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001E90;


_04001E88:	// 4001e88: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001E8C:	// 4001e8c: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001E90:	// 4001e90: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[29].U16[0], state.v[29].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[29].U16[1], state.v[29].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[29].U16[2], state.v[29].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[29].U16[3], state.v[29].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[29].U16[4], state.v[29].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[29].U16[5], state.v[29].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[29].U16[6], state.v[29].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[29].U16[7], state.v[29].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E94:	// 4001e94: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[29].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[29].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[29].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[29].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[29].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[29].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[29].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[29].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001E98:	// 4001e98: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E9C:	// 4001e9c: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[27].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[27].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[27].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[27].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[27].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[27].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[27].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[27].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
	}


_04001EA0:	// 4001ea0: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[8], &state.v[17]);


_04001EA4:	// 4001ea4: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001EA8:	// 4001ea8: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001EAC:	// 4001eac: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	{
		_u32    addr = (0x00000000 + state.r[S5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[29].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[29].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[29].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[29].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[29].U64[0] & temp1);
			value2 |= (state.v[29].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001EB0:	// 4001eb0: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[28].U16[0], state.v[28].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[28].U16[1], state.v[28].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[28].U16[2], state.v[28].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[28].U16[3], state.v[28].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[28].U16[4], state.v[28].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[28].U16[5], state.v[28].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[28].U16[6], state.v[28].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[28].U16[7], state.v[28].U16[7], s2value, 0, 1, 1, 1)
	}


_04001EB4:	// 4001eb4: <22b50010>	ADDI      S5 = S5 + 0010
	state.r[S5] = state.r[S5] + 0x0010;


_04001EB8:	// 4001eb8: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[28].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[28].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[28].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[28].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[28].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[28].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[28].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[28].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001EBC:	// 4001ebc: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001EC0:	// 4001ec0: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 0, 1, 1, 1)
	}


_04001EC4:	// 4001ec4: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_04001EC8:	// 4001ec8: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[26].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 1, 1, 1)
		VMACF_operation(1, state.v[26].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 1, 1, 1)
		VMACF_operation(2, state.v[26].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 1, 1, 1)
		VMACF_operation(3, state.v[26].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 1, 1, 1)
		VMACF_operation(4, state.v[26].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 1, 1, 1)
		VMACF_operation(5, state.v[26].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 1, 1, 1)
		VMACF_operation(6, state.v[26].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 1, 1, 1)
		VMACF_operation(7, state.v[26].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 1, 1, 1)
	}


_04001ECC:	// 4001ecc: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	{
		_u32    addr = (0x00000000 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[28].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[28].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[28].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[28].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[28].U64[0] & temp1);
			value2 |= (state.v[28].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001ED0:	// 4001ed0: <22940010>	ADDI      S4 = S4 + 0010
	state.r[S4] = state.r[S4] + 0x0010;


_04001ED4:	// 4001ed4: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001ED8:	// 4001ed8: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001EDC:	// 4001edc: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[5];
		VADDC_operation(0, state.v[21].U16[0], state.v[21].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[21].U16[1], state.v[21].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[21].U16[2], state.v[21].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[21].U16[3], state.v[21].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[21].U16[4], state.v[21].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[21].U16[5], state.v[21].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[21].U16[6], state.v[21].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[21].U16[7], state.v[21].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001EE0:	// 4001ee0: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[6];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001EE4:	// 4001ee4: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001EE8:	// 4001ee8: <4bb89cd4>	VADDC	vec19 = vec19 + vec24[5]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[2];
		VADDC_operation(0, state.v[19].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[19].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[19].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[19].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[19].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[19].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[19].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[19].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001EEC:	// 4001eec: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001EF0:	// 4001ef0: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001EF0;
	if (state.r[S7] >= 0)
	{
		{
			_u16		flag0 = state.flag[0];
			_u16		s2value = state.v[24].U16[3];
			VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 1, 1, 1)
			VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 1, 1, 1)
			VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 1, 1, 1)
			VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 1, 1, 1)
			VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 1, 1, 1)
			VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 1, 1, 1)
			VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 1, 1, 1)
			VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 1, 1, 1)
			state.flag[0] = 0x0000;
		}
		goto _04001F00;
	}


_04001EF4:	// 4001ef4: <4b989490>	VADD      vec18 = vec18 + vec24[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[3];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001EF8:	// 4001ef8: <090007c1>	J         04001f04
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001F04;


_04001EFC:	// 4001efc: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001F00:	// 4001f00: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001F04:	// 4001f04: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001F08:	// 4001f08: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001F08;
	if (state.r[S6] >= 0)
	{
		{
			_u32 addr = (0x00000000 + state.r[S3]);
			int shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
				state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
				state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
				state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
			}
			else
			{
				_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
				_u64		value1 = Load64_DMEM(addr+8);
				_u64		value2 = Load64_DMEM(addr);
				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & temp1;
				value2 = value2 & temp2;

				state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
				state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
				state.v[27].U64[0] = state.v[27].U64[0] | value1;
				state.v[27].U64[1] = state.v[27].U64[1] | value2;
			}
		}
		goto _04001F18;
	}


_04001F0C:	// 4001f0c: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001F10:	// 4001f10: <090007c7>	J         04001f1c
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001F1C;


_04001F14:	// 4001f14: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001F18:	// 4001f18: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001F1C:	// 4001f1c: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
	}


_04001F20:	// 4001f20: <eb142000>	SQV       [T8 + 0x00] = vec20 <00>
	// In function: Tooie1F20_1F84
	//Call the function: Tooie1F20_1F84
	Tooie1F20_1F84();
	goto _04001F88;

_04001F24:	// 4001f24: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F24);

_04001F28:	// 4001f28: <eb152001>	SQV       [T8 + 0x10] = vec21 <00>
	ErrorToCallFunction(0x04001F28);

_04001F2C:	// 4001f2c: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F2C);

_04001F30:	// 4001f30: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F30);

_04001F34:	// 4001f34: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001F34);

_04001F38:	// 4001f38: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F38);

_04001F3C:	// 4001f3c: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001F3C);

_04001F40:	// 4001f40: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F40);

_04001F44:	// 4001f44: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	ErrorToCallFunction(0x04001F44);

_04001F48:	// 4001f48: <2231fff0>	ADDI      S1 = S1 + fff0
	ErrorToCallFunction(0x04001F48);

_04001F4C:	// 4001f4c: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	ErrorToCallFunction(0x04001F4C);

_04001F50:	// 4001f50: <22100010>	ADDI      S0 = S0 + 0010
	ErrorToCallFunction(0x04001F50);

_04001F54:	// 4001f54: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	ErrorToCallFunction(0x04001F54);

_04001F58:	// 4001f58: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	ErrorToCallFunction(0x04001F58);

_04001F5C:	// 4001f5c: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F5C);

_04001F60:	// 4001f60: <22b50010>	ADDI      S5 = S5 + 0010
	ErrorToCallFunction(0x04001F60);

_04001F64:	// 4001f64: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F64);

_04001F68:	// 4001f68: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	ErrorToCallFunction(0x04001F68);

_04001F6C:	// 4001f6c: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F6C);

_04001F70:	// 4001f70: <22730010>	ADDI      S3 = S3 + 0010
	ErrorToCallFunction(0x04001F70);

_04001F74:	// 4001f74: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F74);

_04001F78:	// 4001f78: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F78);

_04001F7C:	// 4001f7c: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F7C);

_04001F80:	// 4001f80: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	ErrorToCallFunction(0x04001F80);

_04001F84:	// 4001f84: <22940010>	ADDI      S4 = S4 + 0010
	ErrorToCallFunction(0x04001F84);

_04001F88:	// 4001f88: <1a200003>	BLEZ      (R0<=0) --> 1f98
	sp_reg_pc = 0x04001F88;
	if ((_s32)state.r[S1] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S2]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[26].U64[0] & temp1);
				value2 |= (state.v[26].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001F98;
	}


_04001F8C:	// 4001f8c: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F90:	// 4001f90: <090007b9>	J         04001ee4
	state.r[S2] = state.r[S2] + 0x0010;
	goto _04001EE4;


_04001F94:	// 4001f94: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001F98:	// 4001f98: <eb122002>	SQV       [T8 + 0x20] = vec18 <00>
	{
		_u32    addr = (0x00000020 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[18].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[18].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[18].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[18].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[18].U64[0] & temp1);
			value2 |= (state.v[18].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F9C:	// 4001f9c: <eb132003>	SQV       [T8 + 0x30] = vec19 <00>
	{
		_u32    addr = (0x00000030 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[19].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[19].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[19].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[19].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[19].U64[0] & temp1);
			value2 |= (state.v[19].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001FA0:	// 4001fa0: <0d000466>	JAL	    04001198
	{
		_u32    addr = (0x00000040 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[24].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[24].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[24].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[24].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[24].U64[0] & temp1);
			value2 |= (state.v[24].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}
	sp_reg_pc = 0x04001FA0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001FA4:	// 4001fa4: <eb182004>	SQV       [T8 + 0x40] = vec24 <00>
	{
		_u32    addr = (0x00000040 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[24].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[24].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[24].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[24].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[24].U64[0] & temp1);
			value2 |= (state.v[24].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001FA8:	// 4001fa8: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001FAC:	// 4001fac: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001FB0:	// 4001fb0: <c81f2006>	LQV       vec31 <00> = [R0 + 0x60]
	// In function: Tooie1FB0_1FF4
	//Call the function: Tooie1FB0_1FF4
	Tooie1FB0_1FF4();
	goto _04001FF8;

_04001FB4:	// 4001fb4: <3336ffff>	ANDI      S6 = T9 & ffff
	ErrorToCallFunction(0x04001FB4);

_04001FB8:	// 4001fb8: <22d60500>	ADDI      S6 = S6 + 0500
	ErrorToCallFunction(0x04001FB8);

_04001FBC:	// 4001fbc: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001FBC);

_04001FC0:	// 4001fc0: <0019bc02>	SRL       S7 = T9 >> 16
	ErrorToCallFunction(0x04001FC0);

_04001FC4:	// 4001fc4: <22f70500>	ADDI      S7 = S7 + 0500
	ErrorToCallFunction(0x04001FC4);

_04001FC8:	// 4001fc8: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001FC8);

_04001FCC:	// 4001fcc: <489af000>	MTC2      vec30 <00> = K0
	ErrorToCallFunction(0x04001FCC);

_04001FD0:	// 4001fd0: <20150170>	ADDI      S5 = R0 + 0170
	ErrorToCallFunction(0x04001FD0);

_04001FD4:	// 4001fd4: <4bbfe6c0>	VMULF     vec27 = ( acc = (vec28 * vec31[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001FD4);

_04001FD8:	// 4001fd8: <22b5fff0>	ADDI      S5 = S5 + fff0
	ErrorToCallFunction(0x04001FD8);

_04001FDC:	// 4001fdc: <22f70010>	ADDI      S7 = S7 + 0010
	ErrorToCallFunction(0x04001FDC);

_04001FE0:	// 4001fe0: <22d60010>	ADDI      S6 = S6 + 0010
	ErrorToCallFunction(0x04001FE0);

_04001FE4:	// 4001fe4: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16
	ErrorToCallFunction(0x04001FE4);

_04001FE8:	// 4001fe8: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001FE8);

_04001FEC:	// 4001fec: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001FEC);

_04001FF0:	// 4001ff0: <1ea0fff8>	BGTZ      (R0>0) --> 1fd4
	ErrorToCallFunction(0x04001FF0);

_04001FF4:	// 4001ff4: <eadb207f>	SQV       [S6 + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x04001FF4);

_04001FF8:	// 4001ff8: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001FFC:	// 4001ffc: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;

	state.halt = 1;
	return;

	_BuildJumpTable:
	#include "jumptable.h"
	goto _Label;
}
#define _PDData _PD_MP3
void _PD_MP3(_u32 StartAddress)
{
	static _u32	JumpTable[(0x1000 >> 2)];
	static _u32	BuildJumpTable = 0;

	extern int DisableMP3;
	if (DisableMP3)	return;
	if (!BuildJumpTable)
	{
		BuildJumpTable = 1;
		goto _BuildJumpTable;
	}

	_Label:
	StartAddress = JumpTable[(StartAddress & 0x0FFF) >> 2];
	__asm mov eax, StartAddress
	__asm jmp eax


_04001000:	// 4001000: <09000419>	J         04001064
	state.r[AT] = 0 + 0x0FC0;
	goto _04001064;


_04001004:	// 4001004: <20010fc0>	ADDI      AT = R0 + 0fc0
	state.r[AT] = 0 + 0x0FC0;


_04001008:	// 4001008: <8c220010>	LW        V0 = [AT+0010]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000010) & 0xfff);


_0400100C:	// 400100c: <20030f7f>	ADDI      V1 = R0 + 0f7f
	state.r[V1] = 0 + 0x0F7F;


_04001010:	// 4001010: <20071080>	ADDI      A3 = R0 + 1080
	state.r[A3] = 0 + 0x1080;


_04001014:	// 4001014: <40870000>	MTC0      SP memory address = A3
	DMEM_Address = state.r[A3];


_04001018:	// 4001018: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_0400101C:	// 400101c: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001020;
	_PDData(0x04001020);
	return;


_04001020:	// 4001020: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_04001024:	// 4001024: <1480fffe>	BNE       (R0!=A0) --> 1020
	sp_reg_pc = 0x04001024;
	if (state.r[A0] != 0)
	{
		goto _04001020;
	}


_04001028:	// 4001028: <00000000>	NOP       


_0400102C:	// 400102c: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x0400102C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001030:	// 4001030: <00000000>	NOP       


_04001034:	// 4001034: <00e00008>	JR        A3
	{
		_u32	temp = ((state.r[A3] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001038:	// 4001038: <40803800>	MTC0      SP semaphore = R0


_0400103C:	// 400103c: <40082000>	MFC0      A0 = DP CMD DMA start
	state.r[T0] = 0;


_04001040:	// 4001040: <31080080>	ANDI      T0 = T0 & 0080
	state.r[T0] = (state.r[T0] & 0x00000080);


_04001044:	// 4001044: <15000002>	BNE       (R0!=T0) --> 1050
	sp_reg_pc = 0x04001044;
	if (state.r[T0] != 0)
	{
		goto _04001050;
	}


_04001048:	// 4001048: <00000000>	NOP       


_0400104C:	// 400104c: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001050:	// 4001050: <40803800>	MTC0      SP semaphore = R0


_04001054:	// 4001054: <34085200>	ORI       T0 = R0 | 0x5200
	state.r[T0] = (0 | 0x00005200);


_04001058:	// 4001058: <40882000>	MTC0      SP status = T0


_0400105C:	// 400105c: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001060:	// 4001060: <00000000>	NOP       


_04001064:	// 4001064: <8c220004>	LW        V0 = [AT+0004]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000004) & 0xfff);


_04001068:	// 4001068: <30420002>	ANDI      V0 = V0 & 0002
	state.r[V0] = (state.r[V0] & 0x00000002);


_0400106C:	// 400106c: <10400007>	BEQ       (R0==V0) --> 108c
	sp_reg_pc = 0x0400106C;
	if (state.r[V0] == 0)
	{
		goto _0400108C;
	}


_04001070:	// 4001070: <00000000>	NOP       


_04001074:	// 4001074: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x04001074;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_04001078:	// 4001078: <00000000>	NOP       


_0400107C:	// 400107c: <40025800>	MFC0      T3 = SP read DMA length
	state.r[V0] = 0;


_04001080:	// 4001080: <30420100>	ANDI      V0 = V0 & 0100
	state.r[V0] = (state.r[V0] & 0x00000100);


_04001084:	// 4001084: <1c40ffed>	BGTZ      (R0>0) --> 103c
	sp_reg_pc = 0x04001084;
	if((_s32)state.r[V0] > 0)
	{
		goto _0400103C;
	}


_04001088:	// 4001088: <00000000>	NOP       


_0400108C:	// 400108c: <8c220018>	LW        V0 = [AT+0018]
	state.r[V0] = (_s32)Load32_DMEM((state.r[AT] + 0x00000018) & 0xfff);


_04001090:	// 4001090: <8c23001c>	LW        V1 = [AT+001c]
	state.r[V1] = (_s32)Load32_DMEM((state.r[AT] + 0x0000001C) & 0xfff);


_04001094:	// 4001094: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001098:	// 4001098: <401e2800>	MFC0      A1 = unknown
	state.r[S8] = 0;


_0400109C:	// 400109c: <17c0fffe>	BNE       (R0!=S8) --> 1098
	sp_reg_pc = 0x0400109C;
	if (state.r[S8] != 0)
	{
		goto _04001098;
	}


_040010A0:	// 40010a0: <00000000>	NOP       


_040010A4:	// 40010a4: <40800000>	MTC0      SP memory address = R0
	DMEM_Address = 0;


_040010A8:	// 40010a8: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040010AC:	// 40010ac: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x040010AC;
	DMARead(state.r[V1]);


_040010B0:	// 40010b0: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_040010B4:	// 40010b4: <1480fffe>	BNE       (R0!=A0) --> 10b0
	sp_reg_pc = 0x040010B4;
	if (state.r[A0] != 0)
	{
		goto _040010B0;
	}


_040010B8:	// 40010b8: <00000000>	NOP       


_040010BC:	// 40010bc: <0d00040f>	JAL	    0400103c
	sp_reg_pc = 0x040010BC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400103C;


_040010C0:	// 40010c0: <00000000>	NOP       


_040010C4:	// 40010c4: <09000402>	J         04001008
	goto _04001008;


_040010C8:	// 40010c8: <00000000>	NOP       


_040010CC:	// 40010cc: <00000000>	NOP       


_040010D0:	// 40010d0: <ac050ff8>	SW        [R0+0ff8] = A1
	Save32_DMEM((_u32)state.r[A1], (0 + 0x00000FF8) & 0xfff);


_040010D4:	// 40010d4: <0d000454>	JAL	    04001150
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x040010D4;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001150;


_040010D8:	// 40010d8: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_040010DC:	// 40010dc: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_040010E0:	// 40010e0: <1440ffff>	BNE       (R0!=V0) --> 10e0
	sp_reg_pc = 0x040010E0;
	if (state.r[V0] != 0)
	{
		state.r[V0] = 0;
		goto _040010E0;
	}


_040010E4:	// 40010e4: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_040010E8:	// 40010e8: <8fba0000>	LW        K0 = [SP+0000]
	// In function: Main_Parser_Tooie
	//Call the function: Main_Parser_Tooie
	Main_Parser_Tooie();
	goto _04001100;

_040010EC:	// 40010ec: <8fb90004>	LW        T9 = [SP+0004]
	ErrorToCallFunction(0x040010EC);

_040010F0:	// 40010f0: <239c0008>	ADDI      GP = GP + 0008
	ErrorToCallFunction(0x040010F0);

_040010F4:	// 40010f4: <001a0dc2>	SRL       AT = K0 >> 23
	ErrorToCallFunction(0x040010F4);

_040010F8:	// 40010f8: <302100fe>	ANDI      AT = AT & 00fe
	ErrorToCallFunction(0x040010F8);

_040010FC:	// 40010fc: <84210010>	LH        AT = [AT+0010]
	ErrorToCallFunction(0x040010FC);

_04001100:	// 4001100: <00200008>	JR        AT
	{
		_u32	temp = ((state.r[AT] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[K1] = state.r[K1] + 0xFFFFFFF8;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001104:	// 4001104: <237bfff8>	ADDI      K1 = K1 + fff8
	state.r[K1] = state.r[K1] + 0xFFFFFFF8;


_04001108:	// 4001108: <0000000d>	BREAK
	state.halt = 1;
	return;


_0400110C:	// 400110c: <1fc0fff6>	BGTZ      (R0>0) --> 10e8
	sp_reg_pc = 0x0400110C;
	if((_s32)state.r[S8] > 0)
	{
		state.r[SP] = state.r[SP] + 0x0008;
		goto _040010E8;
	}


_04001110:	// 4001110: <23bd0008>	ADDI      SP = SP + 0008
	state.r[SP] = state.r[SP] + 0x0008;


_04001114:	// 4001114: <1b600005>	BLEZ      (R0<=0) --> 112c
	sp_reg_pc = 0x04001114;
	if ((_s32)state.r[K1] <= 0)
	{
		state.r[AT] = (0 | 0x00004000);
		goto _0400112C;
	}


_04001118:	// 4001118: <34014000>	ORI       AT = R0 | 0x4000
	state.r[AT] = (0 | 0x00004000);


_0400111C:	// 400111c: <0d000454>	JAL	    04001150
	state.r[V0] = (0 + state.r[GP]);
	sp_reg_pc = 0x0400111C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001150;


_04001120:	// 4001120: <001c1020>	ADD       V0 = R0+GP
	state.r[V0] = (0 + state.r[GP]);


_04001124:	// 4001124: <09000438>	J         040010e0
	state.r[V0] = 0;
	goto _040010E0;


_04001128:	// 4001128: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_0400112C:	// 400112c: <40023000>	MFC0      A2 = SP read DMA length
	state.r[V0] = 0;


_04001130:	// 4001130: <1440fffe>	BNE       (R0!=V0) --> 112c
	sp_reg_pc = 0x04001130;
	if (state.r[V0] != 0)
	{
		goto _0400112C;
	}


_04001134:	// 4001134: <00000000>	NOP       


_04001138:	// 4001138: <40803800>	MTC0      SP semaphore = R0


_0400113C:	// 400113c: <40812000>	MTC0      SP status = AT


_04001140:	// 4001140: <0000000d>	BREAK
	state.halt = 1;
	return;


_04001144:	// 4001144: <00000000>	NOP       


_04001148:	// 4001148: <1000ffff>	BEQ       (R0==R0) --> 1148
	sp_reg_pc = 0x04001148;
	if (0 == 0)
	{
		goto _04001148;
	}


_0400114C:	// 400114c: <00000000>	NOP       


_04001150:	// 4001150: <23e50000>	ADDI      A1 = RA + 0000
	state.r[A1] = state.r[RA] + 0x0000;


_04001154:	// 4001154: <23630000>	ADDI      V1 = K1 + 0000
	state.r[V1] = state.r[K1] + 0x0000;


_04001158:	// 4001158: <2064fec0>	ADDI      A0 = V1 + fec0
	state.r[A0] = state.r[V1] + 0xFFFFFEC0;


_0400115C:	// 400115c: <18800002>	BLEZ      (R0<=0) --> 1168
	sp_reg_pc = 0x0400115C;
	if ((_s32)state.r[A0] <= 0)
	{
		state.r[AT] = 0 + 0x02C0;
		goto _04001168;
	}


_04001160:	// 4001160: <200102c0>	ADDI      AT = R0 + 02c0
	state.r[AT] = 0 + 0x02C0;


_04001164:	// 4001164: <20030140>	ADDI      V1 = R0 + 0140
	state.r[V1] = 0 + 0x0140;


_04001168:	// 4001168: <207e0000>	ADDI      S8 = V1 + 0000
	state.r[S8] = state.r[V1] + 0x0000;


_0400116C:	// 400116c: <0d00045f>	JAL	    0400117c
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x0400116C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001170:	// 4001170: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001174:	// 4001174: <00a00008>	JR        A1
	{
		_u32	temp = ((state.r[A1] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		state.r[SP] = 0 + 0x02C0;
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001178:	// 4001178: <201d02c0>	ADDI      SP = R0 + 02c0
	state.r[SP] = 0 + 0x02C0;


_0400117C:	// 400117c: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001180:	// 4001180: <1480ffff>	BNE       (R0!=A0) --> 1180
	sp_reg_pc = 0x04001180;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _04001180;
	}


_04001184:	// 4001184: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_04001188:	// 4001188: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_0400118C:	// 400118c: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_04001190:	// 4001190: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		sp_reg_pc = 0x04001194;
		DMARead(state.r[V1]);
		{
			extern int gSwitchToFunction;
			extern void _PD_MP3(_u32 StartAddress);
			if( gSwitchToFunction >= 0 && ucodeInfos[gUcode].DMAMaps[gSwitchToFunction].functionAddress != _PDData )
			{
				int functionToSwitchTo = gSwitchToFunction;
				gSwitchToFunction = -1;
				ucodeInfos[gUcode].DMAMaps[functionToSwitchTo].functionAddress(state.r[RA]);
				return;
			}
		}
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001194:	// 4001194: <40831000>	MTC0      SP read DMA length = V1
	sp_reg_pc = 0x04001194;
	DMARead(state.r[V1]);
	{
		extern int gSwitchToFunction;
		extern void _PD_MP3(_u32 StartAddress);
		if( gSwitchToFunction >= 0 && ucodeInfos[gUcode].DMAMaps[gSwitchToFunction].functionAddress != _PDData )
		{
			int functionToSwitchTo = gSwitchToFunction;
			gSwitchToFunction = -1;
			ucodeInfos[gUcode].DMAMaps[functionToSwitchTo].functionAddress(state.r[RA]);
			return;
		}
	}


_04001198:	// 4001198: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_0400119C:	// 400119c: <1480ffff>	BNE       (R0!=A0) --> 119c
	sp_reg_pc = 0x0400119C;
	if (state.r[A0] != 0)
	{
		state.r[A0] = 0;
		goto _0400119C;
	}


_040011A0:	// 40011a0: <40042800>	MFC0      A1 = SP status
	state.r[A0] = 0;


_040011A4:	// 40011a4: <40810000>	MTC0      SP memory address = AT
	DMEM_Address = state.r[AT];


_040011A8:	// 40011a8: <40820800>	MTC0      SP DRAM DMA address = V0
	RDRAM_Address = (state.r[V0] & 0xFFFFFF);


_040011AC:	// 40011ac: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		DMAWrite(state.r[V1]);
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040011B0:	// 40011b0: <40831800>	MTC0      SP write DMA length = V1
	DMAWrite(state.r[V1]);


_040011B4:	// 40011b4: <40043000>	MFC0      A2 = SP status
	state.r[A0] = 0;


_040011B8:	// 40011b8: <1480fffe>	BNE       (R0!=A0) --> 11b4
	sp_reg_pc = 0x040011B8;
	if (state.r[A0] != 0)
	{
		goto _040011B4;
	}


_040011BC:	// 40011bc: <00000000>	NOP       


_040011C0:	// 40011c0: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_040011C4:	// 40011c4: <00000000>	NOP       


_040011C8:	// 40011c8: <84030004>	LH        V1 = [R0+0004]
	state.r[V1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000004) & 0xfff);


_040011CC:	// 40011cc: <8c020000>	LW        V0 = [R0+0000]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000000) & 0xfff);


_040011D0:	// 40011d0: <8401000e>	LH        AT = [R0+000e]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);


_040011D4:	// 40011d4: <00411020>	ADD       V0 = V0+AT
	state.r[V0] = (state.r[V0] + state.r[AT]);


_040011D8:	// 40011d8: <00611822>	SUB       V1 = V1-AT
	state.r[V1] = (state.r[V1] - state.r[AT]);


_040011DC:	// 40011dc: <84010006>	LH        AT = [R0+0006]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x00000006) & 0xfff);


_040011E0:	// 40011e0: <00411022>	SUB       V0 = V0-AT
	state.r[V0] = (state.r[V0] - state.r[AT]);


_040011E4:	// 40011e4: <00611820>	ADD       V1 = V1+AT
	state.r[V1] = (state.r[V1] + state.r[AT]);


_040011E8:	// 40011e8: <0d00045f>	JAL	    0400117c
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);
	sp_reg_pc = 0x040011E8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_040011EC:	// 40011ec: <8401000e>	LH        AT = [R0+000e]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);


_040011F0:	// 40011f0: <8c020ff8>	LW        V0 = [R0+0ff8]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000FF8) & 0xfff);


_040011F4:	// 40011f4: <20420010>	ADDI      V0 = V0 + 0010
	state.r[V0] = state.r[V0] + 0x0010;


_040011F8:	// 40011f8: <20010010>	ADDI      AT = R0 + 0010
	state.r[AT] = 0 + 0x0010;


_040011FC:	// 40011fc: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x02AF;
	sp_reg_pc = 0x040011FC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001200:	// 4001200: <200302af>	ADDI      V1 = R0 + 02af
	state.r[V1] = 0 + 0x02AF;


_04001204:	// 4001204: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x04001204;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001208:	// 4001208: <00000000>	NOP       


_0400120C:	// 400120c: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001210:	// 4001210: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001214:	// 4001214: <8401000e>	LH        AT = [R0+000e]
	state.r[AT] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000E) & 0xfff);


_04001218:	// 4001218: <8c020008>	LW        V0 = [R0+0008]
	state.r[V0] = (_s32)Load32_DMEM((0 + 0x00000008) & 0xfff);


_0400121C:	// 400121c: <0d00045f>	JAL	    0400117c
	state.r[V1] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000C) & 0xfff);
	sp_reg_pc = 0x0400121C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001220:	// 4001220: <8403000c>	LH        V1 = [R0+000c]
	state.r[V1] = (_s32)(_s16)Load16_DMEM((0 + 0x0000000C) & 0xfff);


_04001224:	// 4001224: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x04001224;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001228:	// 4001228: <00000000>	NOP       


_0400122C:	// 400122c: <0900048e>	J         04001238
	goto _04001238;


_04001230:	// 4001230: <00000000>	NOP       


_04001234:	// 4001234: <00000000>	NOP       


_04001238:	// 4001238: <8c110ff4>	LW        S1 = [R0+0ff4]
	state.r[S1] = (_s32)Load32_DMEM((0 + 0x00000FF4) & 0xfff);


_0400123C:	// 400123c: <8c100ff8>	LW        S0 = [R0+0ff8]
	state.r[S0] = (_s32)Load32_DMEM((0 + 0x00000FF8) & 0xfff);


_04001240:	// 4001240: <8c0f0ffc>	LW        T7 = [R0+0ffc]
	state.r[T7] = (_s32)Load32_DMEM((0 + 0x00000FFC) & 0xfff);


_04001244:	// 4001244: <02201020>	ADD       V0 = S1+R0
	state.r[V0] = (state.r[S1] + 0);


_04001248:	// 4001248: <200108a0>	ADDI      AT = R0 + 08a0
	state.r[AT] = 0 + 0x08A0;


_0400124C:	// 400124c: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x043F;
	sp_reg_pc = 0x0400124C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001250:	// 4001250: <2003043f>	ADDI      V1 = R0 + 043f
	state.r[V1] = 0 + 0x043F;


_04001254:	// 4001254: <02201020>	ADD       V0 = S1+R0
	state.r[V0] = (state.r[S1] + 0);


_04001258:	// 4001258: <200102c0>	ADDI      AT = R0 + 02c0
	state.r[AT] = 0 + 0x02C0;


_0400125C:	// 400125c: <0d000466>	JAL	    04001198
	state.r[V1] = 0 + 0x013F;
	sp_reg_pc = 0x0400125C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001260:	// 4001260: <2003013f>	ADDI      V1 = R0 + 013f
	state.r[V1] = 0 + 0x013F;


_04001264:	// 4001264: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x04001264;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001268:	// 4001268: <00000000>	NOP       


_0400126C:	// 400126c: <220202c0>	ADDI      V0 = S0 + 02c0
	state.r[V0] = state.r[S0] + 0x02C0;


_04001270:	// 4001270: <20010010>	ADDI      AT = R0 + 0010
	state.r[AT] = 0 + 0x0010;


_04001274:	// 4001274: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x088F;
	sp_reg_pc = 0x04001274;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001278:	// 4001278: <2003088f>	ADDI      V1 = R0 + 088f
	state.r[V1] = 0 + 0x088F;


_0400127C:	// 400127c: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x0400127C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001280:	// 4001280: <00000000>	NOP       


_04001284:	// 4001284: <0019aa00>	SLL       S5 = T9 << 8
	state.r[S5] = ((_u32)state.r[T9] << 8);


_04001288:	// 4001288: <0015aa02>	SRL       S5 = S5 >> 8
	state.r[S5] = ((_u32)state.r[S5] >> 8);


_0400128C:	// 400128c: <0015b020>	ADD       S6 = R0+S5
	state.r[S6] = (0 + state.r[S5]);


_04001290:	// 4001290: <20030188>	ADDI      V1 = R0 + 0188
	state.r[V1] = 0 + 0x0188;


_04001294:	// 4001294: <20140480>	ADDI      S4 = R0 + 0480
	state.r[S4] = 0 + 0x0480;


_04001298:	// 4001298: <00151020>	ADD       V0 = R0+S5
	state.r[V0] = (0 + state.r[S5]);


_0400129C:	// 400129c: <22b50008>	ADDI      S5 = S5 + 0008
	state.r[S5] = state.r[S5] + 0x0008;


_040012A0:	// 40012a0: <20010ce8>	ADDI      AT = R0 + 0ce8
	state.r[AT] = 0 + 0x0CE8;


_040012A4:	// 40012a4: <200e08a0>	ADDI      T6 = R0 + 08a0
	state.r[T6] = 0 + 0x08A0;


_040012A8:	// 40012a8: <21cd0220>	ADDI      T5 = T6 + 0220
	state.r[T5] = state.r[T6] + 0x0220;


_040012AC:	// 40012ac: <334c001e>	ANDI      T4 = K0 & 001e
	state.r[T4] = (state.r[K0] & 0x0000001E);


_040012B0:	// 40012b0: <0d00045f>	JAL	    0400117c
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x040012B0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_040012B4:	// 40012b4: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_040012B8:	// 40012b8: <20030180>	ADDI      V1 = R0 + 0180
	state.r[V1] = 0 + 0x0180;


_040012BC:	// 40012bc: <2294fe80>	ADDI      S4 = S4 + fe80
	state.r[S4] = state.r[S4] + 0xFFFFFE80;


_040012C0:	// 40012c0: <20120180>	ADDI      S2 = R0 + 0180
	state.r[S2] = 0 + 0x0180;


_040012C4:	// 40012c4: <20170cf0>	ADDI      S7 = R0 + 0cf0
	state.r[S7] = 0 + 0x0CF0;


_040012C8:	// 40012c8: <0d00046d>	JAL	    040011b4
	state.r[S3] = 0 + 0x0E70;
	sp_reg_pc = 0x040012C8;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_040012CC:	// 40012cc: <20130e70>	ADDI      S3 = R0 + 0e70
	state.r[S3] = 0 + 0x0E70;


_040012D0:	// 40012d0: <31ceffe0>	ANDI      T6 = T6 & ffe0
	state.r[T6] = (state.r[T6] & 0x0000FFE0);


_040012D4:	// 40012d4: <31adffe0>	ANDI      T5 = T5 & ffe0
	state.r[T5] = (state.r[T5] & 0x0000FFE0);


_040012D8:	// 40012d8: <01cc7025>	OR        T6 = T6 | T4
	state.r[T6] = (state.r[T6] | state.r[T4]);


_040012DC:	// 40012dc: <01ac6825>	OR        T5 = T5 | T4
	state.r[T5] = (state.r[T5] | state.r[T4]);


_040012E0:	// 40012e0: <cae00800>	LSV       vec00 <00> = [S7 + 0x00]
	state.v[0].U16[7] = Load16_DMEM(0x00000000 + state.r[S7]);


_040012E4:	// 40012e4: <caff081f>	LSV       vec31 <00> = [S7 + 0x1e]
	state.v[31].U16[7] = Load16_DMEM(0x0000003E + state.r[S7]);


_040012E8:	// 40012e8: <cae10801>	LSV       vec01 <00> = [S7 + 0x02]
	state.v[1].U16[7] = Load16_DMEM(0x00000002 + state.r[S7]);


_040012EC:	// 40012ec: <cafe081e>	LSV       vec30 <00> = [S7 + 0x1c]
	state.v[30].U16[7] = Load16_DMEM(0x0000003C + state.r[S7]);


_040012F0:	// 40012f0: <cae20803>	LSV       vec02 <00> = [S7 + 0x06]
	state.v[2].U16[7] = Load16_DMEM(0x00000006 + state.r[S7]);


_040012F4:	// 40012f4: <cafc081c>	LSV       vec28 <00> = [S7 + 0x18]
	state.v[28].U16[7] = Load16_DMEM(0x00000038 + state.r[S7]);


_040012F8:	// 40012f8: <4a1f0010>	VADD      vec00 = vec00 + vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040012FC:	// 40012fc: <cae30802>	LSV       vec03 <00> = [S7 + 0x04]
	state.v[3].U16[7] = Load16_DMEM(0x00000004 + state.r[S7]);


_04001300:	// 4001300: <cafd081d>	LSV       vec29 <00> = [S7 + 0x1a]
	state.v[29].U16[7] = Load16_DMEM(0x0000003A + state.r[S7]);


_04001304:	// 4001304: <4a1e0850>	VADD      vec01 = vec01 + vec30[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[1].U16[0], state.v[1].U16[0], state.v[30].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[1].U16[1], state.v[1].U16[1], state.v[30].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[1].U16[2], state.v[1].U16[2], state.v[30].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[1].U16[3], state.v[1].U16[3], state.v[30].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[1].U16[4], state.v[1].U16[4], state.v[30].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[1].U16[5], state.v[1].U16[5], state.v[30].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[1].U16[6], state.v[1].U16[6], state.v[30].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[1].U16[7], state.v[1].U16[7], state.v[30].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001308:	// 4001308: <cae40807>	LSV       vec04 <00> = [S7 + 0x0e]
	state.v[4].U16[7] = Load16_DMEM(0x0000000E + state.r[S7]);


_0400130C:	// 400130c: <caf80818>	LSV       vec24 <00> = [S7 + 0x10]
	state.v[24].U16[7] = Load16_DMEM(0x00000030 + state.r[S7]);


_04001310:	// 4001310: <4a1c1090>	VADD      vec02 = vec02 + vec28[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[28].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[28].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[28].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[28].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[28].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[28].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[28].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[28].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001314:	// 4001314: <cae50806>	LSV       vec05 <00> = [S7 + 0x0c]
	state.v[5].U16[7] = Load16_DMEM(0x0000000C + state.r[S7]);


_04001318:	// 4001318: <caf90819>	LSV       vec25 <00> = [S7 + 0x12]
	state.v[25].U16[7] = Load16_DMEM(0x00000032 + state.r[S7]);


_0400131C:	// 400131c: <4a1d18d0>	VADD      vec03 = vec03 + vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[3].U16[0], state.v[3].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[3].U16[1], state.v[3].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[3].U16[2], state.v[3].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[3].U16[3], state.v[3].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[3].U16[4], state.v[3].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[3].U16[5], state.v[3].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[3].U16[6], state.v[3].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[3].U16[7], state.v[3].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001320:	// 4001320: <cae60804>	LSV       vec06 <00> = [S7 + 0x08]
	state.v[6].U16[7] = Load16_DMEM(0x00000008 + state.r[S7]);


_04001324:	// 4001324: <cafb081b>	LSV       vec27 <00> = [S7 + 0x16]
	state.v[27].U16[7] = Load16_DMEM(0x00000036 + state.r[S7]);


_04001328:	// 4001328: <4a182110>	VADD      vec04 = vec04 + vec24[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[24].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[24].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[24].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[24].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[24].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[24].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[24].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[24].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400132C:	// 400132c: <cae70805>	LSV       vec07 <00> = [S7 + 0x0a]
	state.v[7].U16[7] = Load16_DMEM(0x0000000A + state.r[S7]);


_04001330:	// 4001330: <cafa081a>	LSV       vec26 <00> = [S7 + 0x14]
	state.v[26].U16[7] = Load16_DMEM(0x00000034 + state.r[S7]);


_04001334:	// 4001334: <4a192950>	VADD      vec05 = vec05 + vec25[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[5].U16[0], state.v[5].U16[0], state.v[25].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[5].U16[1], state.v[5].U16[1], state.v[25].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[5].U16[2], state.v[5].U16[2], state.v[25].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[5].U16[3], state.v[5].U16[3], state.v[25].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[5].U16[4], state.v[5].U16[4], state.v[25].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[5].U16[5], state.v[5].U16[5], state.v[25].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[5].U16[6], state.v[5].U16[6], state.v[25].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[5].U16[7], state.v[5].U16[7], state.v[25].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001338:	// 4001338: <cae8080f>	LSV       vec08 <00> = [S7 + 0x1e]
	state.v[8].U16[7] = Load16_DMEM(0x0000001E + state.r[S7]);


_0400133C:	// 400133c: <caf00810>	LSV       vec16 <00> = [S7 + 0x00]
	state.v[16].U16[7] = Load16_DMEM(0x00000020 + state.r[S7]);


_04001340:	// 4001340: <4a1b3190>	VADD      vec06 = vec06 + vec27[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[27].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[27].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[27].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[27].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[27].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[27].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[27].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[27].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001344:	// 4001344: <cae9080e>	LSV       vec09 <00> = [S7 + 0x1c]
	state.v[9].U16[7] = Load16_DMEM(0x0000001C + state.r[S7]);


_04001348:	// 4001348: <caf10811>	LSV       vec17 <00> = [S7 + 0x02]
	state.v[17].U16[7] = Load16_DMEM(0x00000022 + state.r[S7]);


_0400134C:	// 400134c: <4a1a39d0>	VADD      vec07 = vec07 + vec26[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[26].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[26].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[26].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[26].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[26].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[26].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[26].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[26].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001350:	// 4001350: <caea080c>	LSV       vec10 <00> = [S7 + 0x18]
	state.v[10].U16[7] = Load16_DMEM(0x00000018 + state.r[S7]);


_04001354:	// 4001354: <caf30813>	LSV       vec19 <00> = [S7 + 0x06]
	state.v[19].U16[7] = Load16_DMEM(0x00000026 + state.r[S7]);


_04001358:	// 4001358: <4a104210>	VADD      vec08 = vec08 + vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[8].U16[0], state.v[8].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[8].U16[1], state.v[8].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[8].U16[2], state.v[8].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[8].U16[3], state.v[8].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[8].U16[4], state.v[8].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[8].U16[5], state.v[8].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[8].U16[6], state.v[8].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[8].U16[7], state.v[8].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400135C:	// 400135c: <caeb080d>	LSV       vec11 <00> = [S7 + 0x1a]
	state.v[11].U16[7] = Load16_DMEM(0x0000001A + state.r[S7]);


_04001360:	// 4001360: <caf20812>	LSV       vec18 <00> = [S7 + 0x04]
	state.v[18].U16[7] = Load16_DMEM(0x00000024 + state.r[S7]);


_04001364:	// 4001364: <4a114a50>	VADD      vec09 = vec09 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[9].U16[0], state.v[9].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[9].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[9].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[9].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[9].U16[4], state.v[9].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[9].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[9].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[9].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001368:	// 4001368: <caec0808>	LSV       vec12 <00> = [S7 + 0x10]
	state.v[12].U16[7] = Load16_DMEM(0x00000010 + state.r[S7]);


_0400136C:	// 400136c: <caf70817>	LSV       vec23 <00> = [S7 + 0x0e]
	state.v[23].U16[7] = Load16_DMEM(0x0000002E + state.r[S7]);


_04001370:	// 4001370: <4a135290>	VADD      vec10 = vec10 + vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001374:	// 4001374: <caed0809>	LSV       vec13 <00> = [S7 + 0x12]
	state.v[13].U16[7] = Load16_DMEM(0x00000012 + state.r[S7]);


_04001378:	// 4001378: <caf60816>	LSV       vec22 <00> = [S7 + 0x0c]
	state.v[22].U16[7] = Load16_DMEM(0x0000002C + state.r[S7]);


_0400137C:	// 400137c: <4a125ad0>	VADD      vec11 = vec11 + vec18[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[18].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[18].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[18].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[18].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[18].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[18].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[18].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[18].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001380:	// 4001380: <caee080b>	LSV       vec14 <00> = [S7 + 0x16]
	state.v[14].U16[7] = Load16_DMEM(0x00000016 + state.r[S7]);


_04001384:	// 4001384: <caf40814>	LSV       vec20 <00> = [S7 + 0x08]
	state.v[20].U16[7] = Load16_DMEM(0x00000028 + state.r[S7]);


_04001388:	// 4001388: <4a176310>	VADD      vec12 = vec12 + vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[12].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[12].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[12].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[12].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[12].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[12].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[12].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[12].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400138C:	// 400138c: <caef080a>	LSV       vec15 <00> = [S7 + 0x14]
	state.v[15].U16[7] = Load16_DMEM(0x00000014 + state.r[S7]);


_04001390:	// 4001390: <caf50815>	LSV       vec21 <00> = [S7 + 0x0a]
	state.v[21].U16[7] = Load16_DMEM(0x0000002A + state.r[S7]);


_04001394:	// 4001394: <4a166b50>	VADD      vec13 = vec13 + vec22[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[22].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[22].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[22].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[22].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[22].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[22].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[22].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[22].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001398:	// 4001398: <4a147390>	VADD      vec14 = vec14 + vec20[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[20].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[20].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[20].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[20].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[20].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[20].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[20].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[20].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400139C:	// 400139c: <4a157bd0>	VADD      vec15 = vec15 + vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[15].U16[0], state.v[15].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[15].U16[1], state.v[15].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[15].U16[2], state.v[15].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[15].U16[3], state.v[15].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[15].U16[4], state.v[15].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[15].U16[5], state.v[15].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[15].U16[6], state.v[15].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[15].U16[7], state.v[15].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013A0:	// 40013a0: <0d0006ac>	JAL	    04001ab0
	sp_reg_pc = 0x040013A0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AB0;


_040013A4:	// 40013a4: <00000000>	NOP       


_040013A8:	// 40013a8: <c8002005>	LQV       vec00 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[0].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[0].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[0].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[0].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[0].U64[0] = state.v[0].U64[0] & ~temp1;
			state.v[0].U64[1] = state.v[0].U64[1] & ~temp2;
			state.v[0].U64[0] = state.v[0].U64[0] | value1;
			state.v[0].U64[1] = state.v[0].U64[1] | value2;
		}
	}


_040013AC:	// 40013ac: <4a1182d1>	VSUB      vec11 = vec16 - vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[16].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[16].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[16].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[16].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[16].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[16].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[16].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[16].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013B0:	// 40013b0: <21c80100>	ADDI      T0 = T6 + 0100
	state.r[T0] = state.r[T6] + 0x0100;


_040013B4:	// 40013b4: <4a118410>	VADD      vec16 = vec16 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[16].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[16].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[16].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[16].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[16].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[16].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[16].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[16].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013B8:	// 40013b8: <21aa0100>	ADDI      T2 = T5 + 0100
	state.r[T2] = state.r[T5] + 0x0100;


_040013BC:	// 40013bc: <4a01086c>	VXOR      vec01 = vec01 xor vec01[<none>]
	rsp_cop2_vxor(&state.v[1], &state.v[1], &state.v[1]);


_040013C0:	// 40013c0: <21c90200>	ADDI      T1 = T6 + 0200
	state.r[T1] = state.r[T6] + 0x0200;


_040013C4:	// 40013c4: <4a100c11>	VSUB      vec16 = vec01 - vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[16].U16[0], state.v[1].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[16].U16[1], state.v[1].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[16].U16[2], state.v[1].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[16].U16[3], state.v[1].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[16].U16[4], state.v[1].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[16].U16[5], state.v[1].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[16].U16[6], state.v[1].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[16].U16[7], state.v[1].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013C8:	// 40013c8: <21ab0200>	ADDI      T3 = T5 + 0200
	state.r[T3] = state.r[T5] + 0x0200;


_040013CC:	// 40013cc: <4b005ac5>	VMUDM     vec11 = ( acc = vec11 * vec00[0] ) >> 16
	{
		_u16		s2value = state.v[0].U16[7];
		VMUDM_operation(0, state.v[11].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[11].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[11].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[11].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[11].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[11].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[11].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[11].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
	}


_040013D0:	// 40013d0: <4a139090>	VADD      vec02 = vec18 + vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[2].U16[0], state.v[18].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[2].U16[1], state.v[18].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[2].U16[2], state.v[18].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[2].U16[3], state.v[18].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[2].U16[4], state.v[18].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[2].U16[5], state.v[18].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[2].U16[6], state.v[18].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[2].U16[7], state.v[18].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013D4:	// 40013d4: <4a1390d1>	VSUB      vec03 = vec18 - vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[3].U16[0], state.v[18].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[3].U16[1], state.v[18].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[3].U16[2], state.v[18].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[3].U16[3], state.v[18].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[3].U16[4], state.v[18].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[3].U16[5], state.v[18].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[3].U16[6], state.v[18].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[3].U16[7], state.v[18].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013D8:	// 40013d8: <e9cb0800>	SSV       [T6 + 0x00] = vec11 <00>
	Save16_DMEM(state.v[11].U16[7], (0x00000000 + state.r[T6]) & 0xfff);


_040013DC:	// 40013dc: <4a0b0ad1>	VSUB      vec11 = vec01 - vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[1].U16[0], state.v[11].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[1].U16[1], state.v[11].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[1].U16[2], state.v[11].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[1].U16[3], state.v[11].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[1].U16[4], state.v[11].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[1].U16[5], state.v[11].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[1].U16[6], state.v[11].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[1].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013E0:	// 40013e0: <e9700800>	SSV       [T3 + 0x00] = vec16 <00>
	Save16_DMEM(state.v[16].U16[7], (0x00000000 + state.r[T3]) & 0xfff);


_040013E4:	// 40013e4: <e9ab0800>	SSV       [T5 + 0x00] = vec11 <00>
	Save16_DMEM(state.v[11].U16[7], (0x00000000 + state.r[T5]) & 0xfff);


_040013E8:	// 40013e8: <4a020891>	VSUB      vec02 = vec01 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[2].U16[0], state.v[1].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[2].U16[1], state.v[1].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[2].U16[2], state.v[1].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[2].U16[3], state.v[1].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[2].U16[4], state.v[1].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[2].U16[5], state.v[1].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[2].U16[6], state.v[1].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[2].U16[7], state.v[1].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013EC:	// 40013ec: <4b601c45>	VMUDM     vec17 = ( acc = vec03 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[17].U16[0], state.v[3].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[3].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[3].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[3].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[3].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[3].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[3].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[3].U16[7], s2value, 0, 1, 0, 1)
	}


_040013F0:	// 40013f0: <e9420800>	SSV       [T2 + 0x00] = vec02 <00>
	Save16_DMEM(state.v[2].U16[7], (0x00000000 + state.r[T2]) & 0xfff);


_040013F4:	// 40013f4: <4b4018cf>	VMADH     vec03 = ( acc+= (vec03 * vec00[2]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[5];
		VMADH_operation(0, state.v[3].U16[0], state.v[3].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[3].U16[1], state.v[3].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[3].U16[2], state.v[3].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[3].U16[3], state.v[3].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[3].U16[4], state.v[3].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[3].U16[5], state.v[3].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[3].U16[6], state.v[3].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[3].U16[7], state.v[3].U16[7], s2value, 1, 0, 0, 1)
	}


_040013F8:	// 40013f8: <4a0218d0>	VADD      vec03 = vec03 + vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[3].U16[0], state.v[3].U16[0], state.v[2].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[3].U16[1], state.v[3].U16[1], state.v[2].U16[1], 0, 0, 0, 1)
		VADD_operation(2, state.v[3].U16[2], state.v[3].U16[2], state.v[2].U16[2], 0, 0, 0, 1)
		VADD_operation(3, state.v[3].U16[3], state.v[3].U16[3], state.v[2].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[3].U16[4], state.v[3].U16[4], state.v[2].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[3].U16[5], state.v[3].U16[5], state.v[2].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[3].U16[6], state.v[3].U16[6], state.v[2].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[3].U16[7], state.v[3].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040013FC:	// 40013fc: <e9030800>	SSV       [T0 + 0x00] = vec03 <00>
	Save16_DMEM(state.v[3].U16[7], (0x00000000 + state.r[T0]) & 0xfff);


_04001400:	// 4001400: <4a15a151>	VSUB      vec05 = vec20 - vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[5].U16[0], state.v[20].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[5].U16[1], state.v[20].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[5].U16[2], state.v[20].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[5].U16[3], state.v[20].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[5].U16[4], state.v[20].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[5].U16[5], state.v[20].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[5].U16[6], state.v[20].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[5].U16[7], state.v[20].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001404:	// 4001404: <4a15a110>	VADD      vec04 = vec20 + vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[4].U16[0], state.v[20].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[4].U16[1], state.v[20].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[4].U16[2], state.v[20].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[4].U16[3], state.v[20].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[4].U16[4], state.v[20].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[4].U16[5], state.v[20].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[4].U16[6], state.v[20].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[4].U16[7], state.v[20].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001408:	// 4001408: <4a17b190>	VADD      vec06 = vec22 + vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[22].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[22].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[22].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[22].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[22].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[22].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[22].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[22].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400140C:	// 400140c: <4a17b1d1>	VSUB      vec07 = vec22 - vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[7].U16[0], state.v[22].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[7].U16[1], state.v[22].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[7].U16[2], state.v[22].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[7].U16[3], state.v[22].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[7].U16[4], state.v[22].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[7].U16[5], state.v[22].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[7].U16[6], state.v[22].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[7].U16[7], state.v[22].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001410:	// 4001410: <4b602c45>	VMUDM     vec17 = ( acc = vec05 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[17].U16[0], state.v[5].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[5].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[5].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[5].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[5].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[5].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[5].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[5].U16[7], s2value, 0, 1, 0, 1)
	}


_04001414:	// 4001414: <4b40294f>	VMADH     vec05 = ( acc+= (vec05 * vec00[2]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[5];
		VMADH_operation(0, state.v[5].U16[0], state.v[5].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[5].U16[1], state.v[5].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[5].U16[2], state.v[5].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[5].U16[3], state.v[5].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[5].U16[4], state.v[5].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[5].U16[5], state.v[5].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[5].U16[6], state.v[5].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[5].U16[7], state.v[5].U16[7], s2value, 1, 0, 0, 1)
	}


_04001418:	// 4001418: <4a040911>	VSUB      vec04 = vec01 - vec04[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[4].U16[0], state.v[1].U16[0], state.v[4].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[4].U16[1], state.v[1].U16[1], state.v[4].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[4].U16[2], state.v[1].U16[2], state.v[4].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[4].U16[3], state.v[1].U16[3], state.v[4].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[4].U16[4], state.v[1].U16[4], state.v[4].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[4].U16[5], state.v[1].U16[5], state.v[4].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[4].U16[6], state.v[1].U16[6], state.v[4].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[4].U16[7], state.v[1].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400141C:	// 400141c: <4ba03c45>	VMUDM     vec17 = ( acc = vec07 * vec00[5] ) >> 16
	{
		_u16		s2value = state.v[0].U16[2];
		VMUDM_operation(0, state.v[17].U16[0], state.v[7].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[7].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[7].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[7].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[7].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[7].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[7].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[7].U16[7], s2value, 0, 1, 0, 1)
	}


_04001420:	// 4001420: <e9640840>	SSV       [T3 + 0x00] = vec04 <00>
	Save16_DMEM(state.v[4].U16[7], (0xFFFFFF80 + state.r[T3]) & 0xfff);


_04001424:	// 4001424: <4b8039cf>	VMADH     vec07 = ( acc+= (vec07 * vec00[4]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[3];
		VMADH_operation(0, state.v[7].U16[0], state.v[7].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[7].U16[1], state.v[7].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[7].U16[2], state.v[7].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[7].U16[3], state.v[7].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[7].U16[4], state.v[7].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[7].U16[5], state.v[7].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[7].U16[6], state.v[7].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[7].U16[7], state.v[7].U16[7], s2value, 1, 0, 0, 1)
	}


_04001428:	// 4001428: <4a042951>	VSUB      vec05 = vec05 - vec04[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[5].U16[0], state.v[5].U16[0], state.v[4].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[5].U16[1], state.v[5].U16[1], state.v[4].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[5].U16[2], state.v[5].U16[2], state.v[4].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[5].U16[3], state.v[5].U16[3], state.v[4].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[5].U16[4], state.v[5].U16[4], state.v[4].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[5].U16[5], state.v[5].U16[5], state.v[4].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[5].U16[6], state.v[5].U16[6], state.v[4].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[5].U16[7], state.v[5].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400142C:	// 400142c: <4a040911>	VSUB      vec04 = vec01 - vec04[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[4].U16[0], state.v[1].U16[0], state.v[4].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[4].U16[1], state.v[1].U16[1], state.v[4].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[4].U16[2], state.v[1].U16[2], state.v[4].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[4].U16[3], state.v[1].U16[3], state.v[4].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[4].U16[4], state.v[1].U16[4], state.v[4].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[4].U16[5], state.v[1].U16[5], state.v[4].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[4].U16[6], state.v[1].U16[6], state.v[4].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[4].U16[7], state.v[1].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001430:	// 4001430: <4a063190>	VADD      vec06 = vec06 + vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001434:	// 4001434: <4a0539d1>	VSUB      vec07 = vec07 - vec05[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001438:	// 4001438: <4a062111>	VSUB      vec04 = vec04 - vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400143C:	// 400143c: <4a062951>	VSUB      vec05 = vec05 - vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[5].U16[0], state.v[5].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[5].U16[1], state.v[5].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[5].U16[2], state.v[5].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[5].U16[3], state.v[5].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[5].U16[4], state.v[5].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[5].U16[5], state.v[5].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[5].U16[6], state.v[5].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[5].U16[7], state.v[5].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001440:	// 4001440: <e9270840>	SSV       [T1 + 0x00] = vec07 <00>
	Save16_DMEM(state.v[7].U16[7], (0xFFFFFF80 + state.r[T1]) & 0xfff);


_04001444:	// 4001444: <e9440840>	SSV       [T2 + 0x00] = vec04 <00>
	Save16_DMEM(state.v[4].U16[7], (0xFFFFFF80 + state.r[T2]) & 0xfff);


_04001448:	// 4001448: <e9050840>	SSV       [T0 + 0x00] = vec05 <00>
	Save16_DMEM(state.v[5].U16[7], (0xFFFFFF80 + state.r[T0]) & 0xfff);


_0400144C:	// 400144c: <4a19c251>	VSUB      vec09 = vec24 - vec25[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[24].U16[0], state.v[25].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[24].U16[1], state.v[25].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[24].U16[2], state.v[25].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[24].U16[3], state.v[25].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[24].U16[4], state.v[25].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[24].U16[5], state.v[25].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[24].U16[6], state.v[25].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[24].U16[7], state.v[25].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001450:	// 4001450: <4a19c210>	VADD      vec08 = vec24 + vec25[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[8].U16[0], state.v[24].U16[0], state.v[25].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[8].U16[1], state.v[24].U16[1], state.v[25].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[8].U16[2], state.v[24].U16[2], state.v[25].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[8].U16[3], state.v[24].U16[3], state.v[25].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[8].U16[4], state.v[24].U16[4], state.v[25].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[8].U16[5], state.v[24].U16[5], state.v[25].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[8].U16[6], state.v[24].U16[6], state.v[25].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[8].U16[7], state.v[24].U16[7], state.v[25].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001454:	// 4001454: <4b604c45>	VMUDM     vec17 = ( acc = vec09 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[17].U16[0], state.v[9].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[9].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[9].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[9].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[9].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[9].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[9].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[9].U16[7], s2value, 0, 1, 0, 1)
	}


_04001458:	// 4001458: <4b404a4f>	VMADH     vec09 = ( acc+= (vec09 * vec00[2]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[5];
		VMADH_operation(0, state.v[9].U16[0], state.v[9].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[9].U16[1], state.v[9].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[9].U16[2], state.v[9].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[9].U16[3], state.v[9].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[9].U16[4], state.v[9].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[9].U16[5], state.v[9].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[9].U16[6], state.v[9].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[9].U16[7], state.v[9].U16[7], s2value, 1, 0, 0, 1)
	}


_0400145C:	// 400145c: <4a1bd2d1>	VSUB      vec11 = vec26 - vec27[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[26].U16[0], state.v[27].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[26].U16[1], state.v[27].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[26].U16[2], state.v[27].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[26].U16[3], state.v[27].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[26].U16[4], state.v[27].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[26].U16[5], state.v[27].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[26].U16[6], state.v[27].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[26].U16[7], state.v[27].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001460:	// 4001460: <4a1bd290>	VADD      vec10 = vec26 + vec27[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[26].U16[0], state.v[27].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[26].U16[1], state.v[27].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[26].U16[2], state.v[27].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[26].U16[3], state.v[27].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[26].U16[4], state.v[27].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[26].U16[5], state.v[27].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[26].U16[6], state.v[27].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[26].U16[7], state.v[27].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001464:	// 4001464: <4a1de351>	VSUB      vec13 = vec28 - vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[28].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[28].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[28].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[28].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[28].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[28].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[28].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[28].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001468:	// 4001468: <4a094090>	VADD      vec02 = vec08 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[2].U16[0], state.v[8].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[2].U16[1], state.v[8].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[2].U16[2], state.v[8].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[2].U16[3], state.v[8].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[2].U16[4], state.v[8].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[2].U16[5], state.v[8].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[2].U16[6], state.v[8].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[2].U16[7], state.v[8].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400146C:	// 400146c: <4ba05c45>	VMUDM     vec17 = ( acc = vec11 * vec00[5] ) >> 16
	{
		_u16		s2value = state.v[0].U16[2];
		VMUDM_operation(0, state.v[17].U16[0], state.v[11].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[11].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[11].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[11].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[11].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[11].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[11].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[11].U16[7], s2value, 0, 1, 0, 1)
	}


_04001470:	// 4001470: <4b805acf>	VMADH     vec11 = ( acc+= (vec11 * vec00[4]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[3];
		VMADH_operation(0, state.v[11].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[11].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[11].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[11].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[11].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[11].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[11].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[11].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
	}


_04001474:	// 4001474: <4ba06c45>	VMUDM     vec17 = ( acc = vec13 * vec00[5] ) >> 16
	{
		_u16		s2value = state.v[0].U16[2];
		VMUDM_operation(0, state.v[17].U16[0], state.v[13].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[13].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[13].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[13].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[13].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[13].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[13].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[13].U16[7], s2value, 0, 1, 0, 1)
	}


_04001478:	// 4001478: <4b806b4f>	VMADH     vec13 = ( acc+= (vec13 * vec00[4]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[3];
		VMADH_operation(0, state.v[13].U16[0], state.v[13].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[13].U16[1], state.v[13].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[13].U16[2], state.v[13].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[13].U16[3], state.v[13].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[13].U16[4], state.v[13].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[13].U16[5], state.v[13].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[13].U16[6], state.v[13].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[13].U16[7], state.v[13].U16[7], s2value, 1, 0, 0, 1)
	}


_0400147C:	// 400147c: <4a1de310>	VADD      vec12 = vec28 + vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[28].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[28].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[28].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[28].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[28].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[28].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[28].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[28].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001480:	// 4001480: <4a025ad0>	VADD      vec11 = vec11 + vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001484:	// 4001484: <4a0a5290>	VADD      vec10 = vec10 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001488:	// 4001488: <4a026b51>	VSUB      vec13 = vec13 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400148C:	// 400148c: <4a0c6310>	VADD      vec12 = vec12 + vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[12].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[12].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[12].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[12].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[12].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[12].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[12].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[12].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001490:	// 4001490: <4a1ff390>	VADD      vec14 = vec30 + vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[30].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[30].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[30].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[30].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[30].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[30].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[30].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[30].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001494:	// 4001494: <4a1ff3d1>	VSUB      vec15 = vec30 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[15].U16[0], state.v[30].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[15].U16[1], state.v[30].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[15].U16[2], state.v[30].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[15].U16[3], state.v[30].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[15].U16[4], state.v[30].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[15].U16[5], state.v[30].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[15].U16[6], state.v[30].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[15].U16[7], state.v[30].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001498:	// 4001498: <4a0a40d0>	VADD      vec03 = vec08 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[3].U16[0], state.v[8].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[3].U16[1], state.v[8].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[3].U16[2], state.v[8].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[3].U16[3], state.v[8].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[3].U16[4], state.v[8].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[3].U16[5], state.v[8].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[3].U16[6], state.v[8].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[3].U16[7], state.v[8].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400149C:	// 400149c: <4a0e7390>	VADD      vec14 = vec14 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014A0:	// 40014a0: <4a0c6b50>	VADD      vec13 = vec13 + vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014A4:	// 40014a4: <4be07c45>	VMUDM     vec17 = ( acc = vec15 * vec00[7] ) >> 16
	{
		_u16		s2value = state.v[0].U16[0];
		VMUDM_operation(0, state.v[17].U16[0], state.v[15].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[15].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[15].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[15].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[15].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[15].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[15].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[15].U16[7], s2value, 0, 1, 0, 1)
	}


_040014A8:	// 40014a8: <4bc07bcf>	VMADH     vec15 = ( acc+= (vec15 * vec00[6]) << 16) >> 16
	{
		_u16		s2value = state.v[0].U16[1];
		VMADH_operation(0, state.v[15].U16[0], state.v[15].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[15].U16[1], state.v[15].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[15].U16[2], state.v[15].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[15].U16[3], state.v[15].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[15].U16[4], state.v[15].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[15].U16[5], state.v[15].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[15].U16[6], state.v[15].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[15].U16[7], state.v[15].U16[7], s2value, 1, 0, 0, 1)
	}


_040014AC:	// 40014ac: <4a0e7390>	VADD      vec14 = vec14 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014B0:	// 40014b0: <4a0b7bd1>	VSUB      vec15 = vec15 - vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[15].U16[0], state.v[15].U16[0], state.v[11].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[15].U16[1], state.v[15].U16[1], state.v[11].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[15].U16[2], state.v[15].U16[2], state.v[11].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[15].U16[3], state.v[15].U16[3], state.v[11].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[15].U16[4], state.v[15].U16[4], state.v[11].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[15].U16[5], state.v[15].U16[5], state.v[11].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[15].U16[6], state.v[15].U16[6], state.v[11].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[15].U16[7], state.v[15].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014B4:	// 40014b4: <4a037391>	VSUB      vec14 = vec14 - vec03[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014B8:	// 40014b8: <4a0e0b91>	VSUB      vec14 = vec01 - vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[14].U16[0], state.v[1].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[14].U16[1], state.v[1].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[14].U16[2], state.v[1].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[14].U16[3], state.v[1].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[14].U16[4], state.v[1].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[14].U16[5], state.v[1].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[14].U16[6], state.v[1].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[14].U16[7], state.v[1].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014BC:	// 40014bc: <4a0a6c51>	VSUB      vec17 = vec13 - vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[17].U16[0], state.v[13].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[17].U16[1], state.v[13].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[17].U16[2], state.v[13].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[17].U16[3], state.v[13].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[17].U16[4], state.v[13].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[17].U16[5], state.v[13].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[17].U16[6], state.v[13].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[17].U16[7], state.v[13].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014C0:	// 40014c0: <4a0e4a50>	VADD      vec09 = vec09 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[9].U16[0], state.v[9].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[9].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[9].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[9].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[9].U16[4], state.v[9].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[9].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[9].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[9].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014C4:	// 40014c4: <4a025ad1>	VSUB      vec11 = vec11 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014C8:	// 40014c8: <e9c90820>	SSV       [T6 + 0x00] = vec09 <00>
	Save16_DMEM(state.v[9].U16[7], (0x00000040 + state.r[T6]) & 0xfff);


_040014CC:	// 40014cc: <4a0d5ad1>	VSUB      vec11 = vec11 - vec13[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[13].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[13].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[13].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[13].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[13].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[13].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[13].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[13].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014D0:	// 40014d0: <e9110860>	SSV       [T0 + 0x00] = vec17 <00>
	Save16_DMEM(state.v[17].U16[7], (0xFFFFFFC0 + state.r[T0]) & 0xfff);


_040014D4:	// 40014d4: <4a0c4311>	VSUB      vec12 = vec08 - vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[12].U16[0], state.v[8].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[12].U16[1], state.v[8].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[12].U16[2], state.v[8].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[12].U16[3], state.v[8].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[12].U16[4], state.v[8].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[12].U16[5], state.v[8].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[12].U16[6], state.v[8].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[12].U16[7], state.v[8].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014D8:	// 40014d8: <e90b0820>	SSV       [T0 + 0x00] = vec11 <00>
	Save16_DMEM(state.v[11].U16[7], (0x00000040 + state.r[T0]) & 0xfff);


_040014DC:	// 40014dc: <4a080a11>	VSUB      vec08 = vec01 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[8].U16[0], state.v[1].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[8].U16[1], state.v[1].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[8].U16[2], state.v[1].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[8].U16[3], state.v[1].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[8].U16[4], state.v[1].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[8].U16[5], state.v[1].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[8].U16[6], state.v[1].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[8].U16[7], state.v[1].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014E0:	// 40014e0: <e92f0860>	SSV       [T1 + 0x00] = vec15 <00>
	Save16_DMEM(state.v[15].U16[7], (0xFFFFFFC0 + state.r[T1]) & 0xfff);


_040014E4:	// 40014e4: <4a0a0a91>	VSUB      vec10 = vec01 - vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[10].U16[0], state.v[1].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[10].U16[1], state.v[1].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[10].U16[2], state.v[1].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[10].U16[3], state.v[1].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[10].U16[4], state.v[1].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[10].U16[5], state.v[1].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[10].U16[6], state.v[1].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[10].U16[7], state.v[1].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014E8:	// 40014e8: <e94c0820>	SSV       [T2 + 0x00] = vec12 <00>
	Save16_DMEM(state.v[12].U16[7], (0x00000040 + state.r[T2]) & 0xfff);


_040014EC:	// 40014ec: <e9680860>	SSV       [T3 + 0x00] = vec08 <00>
	Save16_DMEM(state.v[8].U16[7], (0xFFFFFFC0 + state.r[T3]) & 0xfff);


_040014F0:	// 40014f0: <4a0c5291>	VSUB      vec10 = vec10 - vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040014F4:	// 40014f4: <e9ae0820>	SSV       [T5 + 0x00] = vec14 <00>
	Save16_DMEM(state.v[14].U16[7], (0x00000040 + state.r[T5]) & 0xfff);


_040014F8:	// 40014f8: <e94a0860>	SSV       [T2 + 0x00] = vec10 <00>
	Save16_DMEM(state.v[10].U16[7], (0xFFFFFFC0 + state.r[T2]) & 0xfff);


_040014FC:	// 40014fc: <cae00800>	LSV       vec00 <00> = [S7 + 0x00]
	state.v[0].U16[7] = Load16_DMEM(0x00000000 + state.r[S7]);


_04001500:	// 4001500: <caff081f>	LSV       vec31 <00> = [S7 + 0x1e]
	state.v[31].U16[7] = Load16_DMEM(0x0000003E + state.r[S7]);


_04001504:	// 4001504: <cae10801>	LSV       vec01 <00> = [S7 + 0x02]
	state.v[1].U16[7] = Load16_DMEM(0x00000002 + state.r[S7]);


_04001508:	// 4001508: <cafe081e>	LSV       vec30 <00> = [S7 + 0x1c]
	state.v[30].U16[7] = Load16_DMEM(0x0000003C + state.r[S7]);


_0400150C:	// 400150c: <cae20803>	LSV       vec02 <00> = [S7 + 0x06]
	state.v[2].U16[7] = Load16_DMEM(0x00000006 + state.r[S7]);


_04001510:	// 4001510: <cafc081c>	LSV       vec28 <00> = [S7 + 0x18]
	state.v[28].U16[7] = Load16_DMEM(0x00000038 + state.r[S7]);


_04001514:	// 4001514: <cae30802>	LSV       vec03 <00> = [S7 + 0x04]
	state.v[3].U16[7] = Load16_DMEM(0x00000004 + state.r[S7]);


_04001518:	// 4001518: <cafd081d>	LSV       vec29 <00> = [S7 + 0x1a]
	state.v[29].U16[7] = Load16_DMEM(0x0000003A + state.r[S7]);


_0400151C:	// 400151c: <4a1f0011>	VSUB      vec00 = vec00 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001520:	// 4001520: <c81f2003>	LQV       vec31 <00> = [R0 + 0x30]
	{
		_u32 addr = (0x00000030 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001524:	// 4001524: <4a1e0851>	VSUB      vec01 = vec01 - vec30[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[1].U16[0], state.v[1].U16[0], state.v[30].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[1].U16[1], state.v[1].U16[1], state.v[30].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[1].U16[2], state.v[1].U16[2], state.v[30].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[1].U16[3], state.v[1].U16[3], state.v[30].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[1].U16[4], state.v[1].U16[4], state.v[30].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[1].U16[5], state.v[1].U16[5], state.v[30].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[1].U16[6], state.v[1].U16[6], state.v[30].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[1].U16[7], state.v[1].U16[7], state.v[30].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001528:	// 4001528: <4a1c1091>	VSUB      vec02 = vec02 - vec28[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[28].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[28].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[28].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[28].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[28].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[28].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[28].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[28].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400152C:	// 400152c: <cae40807>	LSV       vec04 <00> = [S7 + 0x0e]
	state.v[4].U16[7] = Load16_DMEM(0x0000000E + state.r[S7]);


_04001530:	// 4001530: <4a1d18d1>	VSUB      vec03 = vec03 - vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[3].U16[0], state.v[3].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[3].U16[1], state.v[3].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[3].U16[2], state.v[3].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[3].U16[3], state.v[3].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[3].U16[4], state.v[3].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[3].U16[5], state.v[3].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[3].U16[6], state.v[3].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[3].U16[7], state.v[3].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001534:	// 4001534: <caf80818>	LSV       vec24 <00> = [S7 + 0x10]
	state.v[24].U16[7] = Load16_DMEM(0x00000030 + state.r[S7]);


_04001538:	// 4001538: <4b1f0005>	VMUDM     vec00 = ( acc = vec00 * vec31[0] ) >> 16
	{
		_u16		s2value = state.v[31].U16[7];
		VMUDM_operation(0, state.v[0].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[0].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[0].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[0].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[0].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[0].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[0].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[0].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
	}


_0400153C:	// 400153c: <cae50806>	LSV       vec05 <00> = [S7 + 0x0c]
	state.v[5].U16[7] = Load16_DMEM(0x0000000C + state.r[S7]);


_04001540:	// 4001540: <4b3f0845>	VMUDM     vec01 = ( acc = vec01 * vec31[1] ) >> 16
	{
		_u16		s2value = state.v[31].U16[6];
		VMUDM_operation(0, state.v[1].U16[0], state.v[1].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[1].U16[1], state.v[1].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[1].U16[2], state.v[1].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[1].U16[3], state.v[1].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[1].U16[4], state.v[1].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[1].U16[5], state.v[1].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[1].U16[6], state.v[1].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[1].U16[7], state.v[1].U16[7], s2value, 1, 0, 0, 1)
	}


_04001544:	// 4001544: <caf90819>	LSV       vec25 <00> = [S7 + 0x12]
	state.v[25].U16[7] = Load16_DMEM(0x00000032 + state.r[S7]);


_04001548:	// 4001548: <4b7f1085>	VMUDM     vec02 = ( acc = vec02 * vec31[3] ) >> 16
	{
		_u16		s2value = state.v[31].U16[4];
		VMUDM_operation(0, state.v[2].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[2].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[2].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[2].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[2].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[2].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[2].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[2].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
	}


_0400154C:	// 400154c: <cae60804>	LSV       vec06 <00> = [S7 + 0x08]
	state.v[6].U16[7] = Load16_DMEM(0x00000008 + state.r[S7]);


_04001550:	// 4001550: <4b5f18c5>	VMUDM     vec03 = ( acc = vec03 * vec31[2] ) >> 16
	{
		_u16		s2value = state.v[31].U16[5];
		VMUDM_operation(0, state.v[3].U16[0], state.v[3].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[3].U16[1], state.v[3].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[3].U16[2], state.v[3].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[3].U16[3], state.v[3].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[3].U16[4], state.v[3].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[3].U16[5], state.v[3].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[3].U16[6], state.v[3].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[3].U16[7], state.v[3].U16[7], s2value, 1, 0, 0, 1)
	}


_04001554:	// 4001554: <cafb081b>	LSV       vec27 <00> = [S7 + 0x16]
	state.v[27].U16[7] = Load16_DMEM(0x00000036 + state.r[S7]);


_04001558:	// 4001558: <4a000010>	VADD      vec00 = vec00 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400155C:	// 400155c: <cae70805>	LSV       vec07 <00> = [S7 + 0x0a]
	state.v[7].U16[7] = Load16_DMEM(0x0000000A + state.r[S7]);


_04001560:	// 4001560: <4a010850>	VADD      vec01 = vec01 + vec01[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[1].U16[0], state.v[1].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[1].U16[1], state.v[1].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[1].U16[2], state.v[1].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[1].U16[3], state.v[1].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[1].U16[4], state.v[1].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[1].U16[5], state.v[1].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[1].U16[6], state.v[1].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[1].U16[7], state.v[1].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001564:	// 4001564: <cafa081a>	LSV       vec26 <00> = [S7 + 0x14]
	state.v[26].U16[7] = Load16_DMEM(0x00000034 + state.r[S7]);


_04001568:	// 4001568: <4a021090>	VADD      vec02 = vec02 + vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400156C:	// 400156c: <c81e2004>	LQV       vec30 <00> = [R0 + 0x40]
	{
		_u32 addr = (0x00000040 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001570:	// 4001570: <4a0318d0>	VADD      vec03 = vec03 + vec03[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[3].U16[0], state.v[3].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[3].U16[1], state.v[3].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[3].U16[2], state.v[3].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[3].U16[3], state.v[3].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[3].U16[4], state.v[3].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[3].U16[5], state.v[3].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[3].U16[6], state.v[3].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[3].U16[7], state.v[3].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001574:	// 4001574: <cae8080f>	LSV       vec08 <00> = [S7 + 0x1e]
	state.v[8].U16[7] = Load16_DMEM(0x0000001E + state.r[S7]);


_04001578:	// 4001578: <4a182111>	VSUB      vec04 = vec04 - vec24[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[24].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[24].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[24].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[24].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[24].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[24].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[24].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[24].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400157C:	// 400157c: <caf00810>	LSV       vec16 <00> = [S7 + 0x00]
	state.v[16].U16[7] = Load16_DMEM(0x00000020 + state.r[S7]);


_04001580:	// 4001580: <4a192951>	VSUB      vec05 = vec05 - vec25[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[5].U16[0], state.v[5].U16[0], state.v[25].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[5].U16[1], state.v[5].U16[1], state.v[25].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[5].U16[2], state.v[5].U16[2], state.v[25].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[5].U16[3], state.v[5].U16[3], state.v[25].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[5].U16[4], state.v[5].U16[4], state.v[25].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[5].U16[5], state.v[5].U16[5], state.v[25].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[5].U16[6], state.v[5].U16[6], state.v[25].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[5].U16[7], state.v[5].U16[7], state.v[25].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001584:	// 4001584: <cae9080e>	LSV       vec09 <00> = [S7 + 0x1c]
	state.v[9].U16[7] = Load16_DMEM(0x0000001C + state.r[S7]);


_04001588:	// 4001588: <4a1b3191>	VSUB      vec06 = vec06 - vec27[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[27].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[27].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[27].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[27].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[27].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[27].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[27].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[27].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400158C:	// 400158c: <caf10811>	LSV       vec17 <00> = [S7 + 0x02]
	state.v[17].U16[7] = Load16_DMEM(0x00000022 + state.r[S7]);


_04001590:	// 4001590: <4a1a39d1>	VSUB      vec07 = vec07 - vec26[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[26].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[26].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[26].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[26].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[26].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[26].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[26].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[26].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001594:	// 4001594: <caea080c>	LSV       vec10 <00> = [S7 + 0x18]
	state.v[10].U16[7] = Load16_DMEM(0x00000018 + state.r[S7]);


_04001598:	// 4001598: <4bff2105>	VMUDM     vec04 = ( acc = vec04 * vec31[7] ) >> 16
	{
		_u16		s2value = state.v[31].U16[0];
		VMUDM_operation(0, state.v[4].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[4].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[4].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[4].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[4].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[4].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[4].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[4].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
	}


_0400159C:	// 400159c: <caf30813>	LSV       vec19 <00> = [S7 + 0x06]
	state.v[19].U16[7] = Load16_DMEM(0x00000026 + state.r[S7]);


_040015A0:	// 40015a0: <4bdf2945>	VMUDM     vec05 = ( acc = vec05 * vec31[6] ) >> 16
	{
		_u16		s2value = state.v[31].U16[1];
		VMUDM_operation(0, state.v[5].U16[0], state.v[5].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[5].U16[1], state.v[5].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[5].U16[2], state.v[5].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[5].U16[3], state.v[5].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[5].U16[4], state.v[5].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[5].U16[5], state.v[5].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[5].U16[6], state.v[5].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[5].U16[7], state.v[5].U16[7], s2value, 1, 0, 0, 1)
	}


_040015A4:	// 40015a4: <caeb080d>	LSV       vec11 <00> = [S7 + 0x1a]
	state.v[11].U16[7] = Load16_DMEM(0x0000001A + state.r[S7]);


_040015A8:	// 40015a8: <4b9f3185>	VMUDM     vec06 = ( acc = vec06 * vec31[4] ) >> 16
	{
		_u16		s2value = state.v[31].U16[3];
		VMUDM_operation(0, state.v[6].U16[0], state.v[6].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[6].U16[1], state.v[6].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[6].U16[2], state.v[6].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[6].U16[3], state.v[6].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[6].U16[4], state.v[6].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[6].U16[5], state.v[6].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[6].U16[6], state.v[6].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[6].U16[7], state.v[6].U16[7], s2value, 1, 0, 0, 1)
	}


_040015AC:	// 40015ac: <caf20812>	LSV       vec18 <00> = [S7 + 0x04]
	state.v[18].U16[7] = Load16_DMEM(0x00000024 + state.r[S7]);


_040015B0:	// 40015b0: <4bbf39c5>	VMUDM     vec07 = ( acc = vec07 * vec31[5] ) >> 16
	{
		_u16		s2value = state.v[31].U16[2];
		VMUDM_operation(0, state.v[7].U16[0], state.v[7].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[7].U16[1], state.v[7].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[7].U16[2], state.v[7].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[7].U16[3], state.v[7].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[7].U16[4], state.v[7].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[7].U16[5], state.v[7].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[7].U16[6], state.v[7].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[7].U16[7], state.v[7].U16[7], s2value, 1, 0, 0, 1)
	}


_040015B4:	// 40015b4: <4a042110>	VADD      vec04 = vec04 + vec04[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[4].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[4].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[4].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[4].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[4].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[4].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[4].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015B8:	// 40015b8: <4a052950>	VADD      vec05 = vec05 + vec05[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[5].U16[0], state.v[5].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[5].U16[1], state.v[5].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[5].U16[2], state.v[5].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[5].U16[3], state.v[5].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[5].U16[4], state.v[5].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[5].U16[5], state.v[5].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[5].U16[6], state.v[5].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[5].U16[7], state.v[5].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015BC:	// 40015bc: <4a063190>	VADD      vec06 = vec06 + vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015C0:	// 40015c0: <4a0739d0>	VADD      vec07 = vec07 + vec07[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[7].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[7].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[7].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[7].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[7].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[7].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[7].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[7].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015C4:	// 40015c4: <caec0808>	LSV       vec12 <00> = [S7 + 0x10]
	state.v[12].U16[7] = Load16_DMEM(0x00000010 + state.r[S7]);


_040015C8:	// 40015c8: <4a104211>	VSUB      vec08 = vec08 - vec16[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[8].U16[0], state.v[8].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[8].U16[1], state.v[8].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[8].U16[2], state.v[8].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[8].U16[3], state.v[8].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[8].U16[4], state.v[8].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[8].U16[5], state.v[8].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[8].U16[6], state.v[8].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[8].U16[7], state.v[8].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015CC:	// 40015cc: <caf70817>	LSV       vec23 <00> = [S7 + 0x0e]
	state.v[23].U16[7] = Load16_DMEM(0x0000002E + state.r[S7]);


_040015D0:	// 40015d0: <4a114a51>	VSUB      vec09 = vec09 - vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[9].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[9].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[9].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[9].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[9].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[9].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[9].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[9].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015D4:	// 40015d4: <caed0809>	LSV       vec13 <00> = [S7 + 0x12]
	state.v[13].U16[7] = Load16_DMEM(0x00000012 + state.r[S7]);


_040015D8:	// 40015d8: <4a135291>	VSUB      vec10 = vec10 - vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015DC:	// 40015dc: <caf60816>	LSV       vec22 <00> = [S7 + 0x0c]
	state.v[22].U16[7] = Load16_DMEM(0x0000002C + state.r[S7]);


_040015E0:	// 40015e0: <4a125ad1>	VSUB      vec11 = vec11 - vec18[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[18].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[18].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[18].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[18].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[18].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[18].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[18].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[18].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040015E4:	// 40015e4: <caee080b>	LSV       vec14 <00> = [S7 + 0x16]
	state.v[14].U16[7] = Load16_DMEM(0x00000016 + state.r[S7]);


_040015E8:	// 40015e8: <4bfe4205>	VMUDM     vec08 = ( acc = vec08 * vec30[7] ) >> 16
	{
		_u16		s2value = state.v[30].U16[0];
		VMUDM_operation(0, state.v[8].U16[0], state.v[8].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[8].U16[1], state.v[8].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[8].U16[2], state.v[8].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[8].U16[3], state.v[8].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[8].U16[4], state.v[8].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[8].U16[5], state.v[8].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[8].U16[6], state.v[8].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[8].U16[7], state.v[8].U16[7], s2value, 1, 0, 0, 1)
	}


_040015EC:	// 40015ec: <caf40814>	LSV       vec20 <00> = [S7 + 0x08]
	state.v[20].U16[7] = Load16_DMEM(0x00000028 + state.r[S7]);


_040015F0:	// 40015f0: <4bde4a45>	VMUDM     vec09 = ( acc = vec09 * vec30[6] ) >> 16
	{
		_u16		s2value = state.v[30].U16[1];
		VMUDM_operation(0, state.v[9].U16[0], state.v[9].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[9].U16[1], state.v[9].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[9].U16[2], state.v[9].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[9].U16[3], state.v[9].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[9].U16[4], state.v[9].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[9].U16[5], state.v[9].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[9].U16[6], state.v[9].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[9].U16[7], state.v[9].U16[7], s2value, 1, 0, 0, 1)
	}


_040015F4:	// 40015f4: <caef080a>	LSV       vec15 <00> = [S7 + 0x14]
	state.v[15].U16[7] = Load16_DMEM(0x00000014 + state.r[S7]);


_040015F8:	// 40015f8: <4b9e5285>	VMUDM     vec10 = ( acc = vec10 * vec30[4] ) >> 16
	{
		_u16		s2value = state.v[30].U16[3];
		VMUDM_operation(0, state.v[10].U16[0], state.v[10].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[10].U16[1], state.v[10].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[10].U16[2], state.v[10].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[10].U16[3], state.v[10].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[10].U16[4], state.v[10].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[10].U16[5], state.v[10].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[10].U16[6], state.v[10].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[10].U16[7], state.v[10].U16[7], s2value, 1, 0, 0, 1)
	}


_040015FC:	// 40015fc: <caf50815>	LSV       vec21 <00> = [S7 + 0x0a]
	state.v[21].U16[7] = Load16_DMEM(0x0000002A + state.r[S7]);


_04001600:	// 4001600: <4bbe5ac5>	VMUDM     vec11 = ( acc = vec11 * vec30[5] ) >> 16
	{
		_u16		s2value = state.v[30].U16[2];
		VMUDM_operation(0, state.v[11].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[11].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[11].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[11].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[11].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[11].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[11].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[11].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
	}


_04001604:	// 4001604: <4a176311>	VSUB      vec12 = vec12 - vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[12].U16[0], state.v[12].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[12].U16[1], state.v[12].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[12].U16[2], state.v[12].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[12].U16[3], state.v[12].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[12].U16[4], state.v[12].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[12].U16[5], state.v[12].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[12].U16[6], state.v[12].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[12].U16[7], state.v[12].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001608:	// 4001608: <4a166b51>	VSUB      vec13 = vec13 - vec22[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[22].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[22].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[22].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[22].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[22].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[22].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[22].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[22].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400160C:	// 400160c: <4a147391>	VSUB      vec14 = vec14 - vec20[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[20].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[20].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[20].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[20].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[20].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[20].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[20].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[20].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001610:	// 4001610: <4a157bd1>	VSUB      vec15 = vec15 - vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[15].U16[0], state.v[15].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[15].U16[1], state.v[15].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[15].U16[2], state.v[15].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[15].U16[3], state.v[15].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[15].U16[4], state.v[15].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[15].U16[5], state.v[15].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[15].U16[6], state.v[15].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[15].U16[7], state.v[15].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001614:	// 4001614: <4b1e6305>	VMUDM     vec12 = ( acc = vec12 * vec30[0] ) >> 16
	{
		_u16		s2value = state.v[30].U16[7];
		VMUDM_operation(0, state.v[12].U16[0], state.v[12].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[12].U16[1], state.v[12].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[12].U16[2], state.v[12].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[12].U16[3], state.v[12].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[12].U16[4], state.v[12].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[12].U16[5], state.v[12].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[12].U16[6], state.v[12].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[12].U16[7], state.v[12].U16[7], s2value, 1, 0, 0, 1)
	}


_04001618:	// 4001618: <4b3e6b45>	VMUDM     vec13 = ( acc = vec13 * vec30[1] ) >> 16
	{
		_u16		s2value = state.v[30].U16[6];
		VMUDM_operation(0, state.v[13].U16[0], state.v[13].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[13].U16[1], state.v[13].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[13].U16[2], state.v[13].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[13].U16[3], state.v[13].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[13].U16[4], state.v[13].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[13].U16[5], state.v[13].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[13].U16[6], state.v[13].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[13].U16[7], state.v[13].U16[7], s2value, 1, 0, 0, 1)
	}


_0400161C:	// 400161c: <4b7e7385>	VMUDM     vec14 = ( acc = vec14 * vec30[3] ) >> 16
	{
		_u16		s2value = state.v[30].U16[4];
		VMUDM_operation(0, state.v[14].U16[0], state.v[14].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[14].U16[1], state.v[14].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[14].U16[2], state.v[14].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[14].U16[3], state.v[14].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[14].U16[4], state.v[14].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[14].U16[5], state.v[14].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[14].U16[6], state.v[14].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[14].U16[7], state.v[14].U16[7], s2value, 1, 0, 0, 1)
	}


_04001620:	// 4001620: <4b5e7bc5>	VMUDM     vec15 = ( acc = vec15 * vec30[2] ) >> 16
	{
		_u16		s2value = state.v[30].U16[5];
		VMUDM_operation(0, state.v[15].U16[0], state.v[15].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[15].U16[1], state.v[15].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[15].U16[2], state.v[15].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[15].U16[3], state.v[15].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[15].U16[4], state.v[15].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[15].U16[5], state.v[15].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[15].U16[6], state.v[15].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[15].U16[7], state.v[15].U16[7], s2value, 1, 1, 1, 1)
	}


_04001624:	// 4001624: <4a0c6310>	VADD      vec12 = vec12 + vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[12].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[12].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[12].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[12].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[12].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[12].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[12].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[12].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001628:	// 4001628: <4a0d6b50>	VADD      vec13 = vec13 + vec13[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[13].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[13].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[13].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[13].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[13].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[13].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[13].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[13].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400162C:	// 400162c: <4a0f7bd0>	VADD      vec15 = vec15 + vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[15].U16[0], state.v[15].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[15].U16[1], state.v[15].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[15].U16[2], state.v[15].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[15].U16[3], state.v[15].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[15].U16[4], state.v[15].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[15].U16[5], state.v[15].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[15].U16[6], state.v[15].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[15].U16[7], state.v[15].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001630:	// 4001630: <0d0006ac>	JAL	    04001ab0
	sp_reg_pc = 0x04001630;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001AB0;


_04001634:	// 4001634: <00000000>	NOP       


_04001638:	// 4001638: <4a139090>	VADD      vec02 = vec18 + vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[2].U16[0], state.v[18].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[2].U16[1], state.v[18].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[2].U16[2], state.v[18].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[2].U16[3], state.v[18].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[2].U16[4], state.v[18].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[2].U16[5], state.v[18].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[2].U16[6], state.v[18].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[2].U16[7], state.v[18].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400163C:	// 400163c: <4a1390d1>	VSUB      vec03 = vec18 - vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[3].U16[0], state.v[18].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[3].U16[1], state.v[18].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[3].U16[2], state.v[18].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[3].U16[3], state.v[18].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[3].U16[4], state.v[18].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[3].U16[5], state.v[18].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[3].U16[6], state.v[18].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[3].U16[7], state.v[18].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001640:	// 4001640: <c8132005>	LQV       vec19 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[19].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[19].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[19].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[19].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[19].U64[0] = state.v[19].U64[0] & ~temp1;
			state.v[19].U64[1] = state.v[19].U64[1] & ~temp2;
			state.v[19].U64[0] = state.v[19].U64[0] | value1;
			state.v[19].U64[1] = state.v[19].U64[1] | value2;
		}
	}


_04001644:	// 4001644: <4b138005>	VMUDM     vec00 = ( acc = vec16 * vec19[0] ) >> 16
	{
		_u16		s2value = state.v[19].U16[7];
		VMUDM_operation(0, state.v[0].U16[0], state.v[16].U16[0], s2value, 0, 1, 1, 1)
		VMUDM_operation(1, state.v[0].U16[1], state.v[16].U16[1], s2value, 0, 1, 1, 1)
		VMUDM_operation(2, state.v[0].U16[2], state.v[16].U16[2], s2value, 0, 1, 1, 1)
		VMUDM_operation(3, state.v[0].U16[3], state.v[16].U16[3], s2value, 0, 1, 1, 1)
		VMUDM_operation(4, state.v[0].U16[4], state.v[16].U16[4], s2value, 0, 1, 1, 1)
		VMUDM_operation(5, state.v[0].U16[5], state.v[16].U16[5], s2value, 0, 1, 1, 1)
		VMUDM_operation(6, state.v[0].U16[6], state.v[16].U16[6], s2value, 0, 1, 1, 1)
		VMUDM_operation(7, state.v[0].U16[7], state.v[16].U16[7], s2value, 0, 1, 1, 1)
	}


_04001648:	// 4001648: <c8122002>	LQV       vec18 <00> = [R0 + 0x20]
	{
		_u32 addr = (0x00000020 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[18].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[18].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[18].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[18].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[18].U64[0] = state.v[18].U64[0] & ~temp1;
			state.v[18].U64[1] = state.v[18].U64[1] & ~temp2;
			state.v[18].U64[0] = state.v[18].U64[0] | value1;
			state.v[18].U64[1] = state.v[18].U64[1] | value2;
		}
	}


_0400164C:	// 400164c: <4b338848>	VMACF     vec01 = ( acc += (vec17 * vec19[1]) << 1 ) >> 16
	{
		_u16		s2value = state.v[19].U16[6];
		VMACF_operation(0, state.v[1].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VMACF_operation(1, state.v[1].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VMACF_operation(2, state.v[1].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VMACF_operation(3, state.v[1].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VMACF_operation(4, state.v[1].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VMACF_operation(5, state.v[1].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VMACF_operation(6, state.v[1].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VMACF_operation(7, state.v[1].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
	}


_04001650:	// 4001650: <4bd28005>	VMUDM     vec00 = ( acc = vec16 * vec18[6] ) >> 16
	{
		_u16		s2value = state.v[18].U16[1];
		VMUDM_operation(0, state.v[0].U16[0], state.v[16].U16[0], s2value, 0, 1, 1, 1)
		VMUDM_operation(1, state.v[0].U16[1], state.v[16].U16[1], s2value, 0, 1, 1, 1)
		VMUDM_operation(2, state.v[0].U16[2], state.v[16].U16[2], s2value, 0, 1, 1, 1)
		VMUDM_operation(3, state.v[0].U16[3], state.v[16].U16[3], s2value, 0, 1, 1, 1)
		VMUDM_operation(4, state.v[0].U16[4], state.v[16].U16[4], s2value, 0, 1, 1, 1)
		VMUDM_operation(5, state.v[0].U16[5], state.v[16].U16[5], s2value, 0, 1, 1, 1)
		VMUDM_operation(6, state.v[0].U16[6], state.v[16].U16[6], s2value, 0, 1, 1, 1)
		VMUDM_operation(7, state.v[0].U16[7], state.v[16].U16[7], s2value, 0, 1, 1, 1)
	}


_04001654:	// 4001654: <4bd2880d>	VMADM     vec00 = ( acc+= vec17 * vec18[6] ) >> 16
	{
		_u16		s2value = state.v[18].U16[1];
		VMADM_operation(0, state.v[0].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VMADM_operation(1, state.v[0].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VMADM_operation(2, state.v[0].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VMADM_operation(3, state.v[0].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VMADM_operation(4, state.v[0].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VMADM_operation(5, state.v[0].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VMADM_operation(6, state.v[0].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VMADM_operation(7, state.v[0].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
	}


_04001658:	// 4001658: <4a10842c>	VXOR      vec16 = vec16 xor vec16[<none>]
	rsp_cop2_vxor(&state.v[16], &state.v[16], &state.v[16]);


_0400165C:	// 400165c: <4a028091>	VSUB      vec02 = vec16 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[2].U16[0], state.v[16].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[2].U16[1], state.v[16].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[2].U16[2], state.v[16].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[2].U16[3], state.v[16].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[2].U16[4], state.v[16].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[2].U16[5], state.v[16].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[2].U16[6], state.v[16].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[2].U16[7], state.v[16].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001660:	// 4001660: <4b731c45>	VMUDM     vec17 = ( acc = vec03 * vec19[3] ) >> 16
	{
		_u16		s2value = state.v[19].U16[4];
		VMUDM_operation(0, state.v[17].U16[0], state.v[3].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[3].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[3].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[3].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[3].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[3].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[3].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[3].U16[7], s2value, 0, 1, 0, 1)
	}


_04001664:	// 4001664: <4b5318cf>	VMADH     vec03 = ( acc+= (vec03 * vec19[2]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[5];
		VMADH_operation(0, state.v[3].U16[0], state.v[3].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[3].U16[1], state.v[3].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[3].U16[2], state.v[3].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[3].U16[3], state.v[3].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[3].U16[4], state.v[3].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[3].U16[5], state.v[3].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[3].U16[6], state.v[3].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[3].U16[7], state.v[3].U16[7], s2value, 1, 0, 0, 1)
	}


_04001668:	// 4001668: <4a15a110>	VADD      vec04 = vec20 + vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[4].U16[0], state.v[20].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[4].U16[1], state.v[20].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[4].U16[2], state.v[20].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[4].U16[3], state.v[20].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[4].U16[4], state.v[20].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[4].U16[5], state.v[20].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[4].U16[6], state.v[20].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[4].U16[7], state.v[20].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400166C:	// 400166c: <4a002110>	VADD      vec04 = vec04 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001670:	// 4001670: <4a15a151>	VSUB      vec05 = vec20 - vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[5].U16[0], state.v[20].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[5].U16[1], state.v[20].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[5].U16[2], state.v[20].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[5].U16[3], state.v[20].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[5].U16[4], state.v[20].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[5].U16[5], state.v[20].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[5].U16[6], state.v[20].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[5].U16[7], state.v[20].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001674:	// 4001674: <4b732c45>	VMUDM     vec17 = ( acc = vec05 * vec19[3] ) >> 16
	{
		_u16		s2value = state.v[19].U16[4];
		VMUDM_operation(0, state.v[17].U16[0], state.v[5].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[5].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[5].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[5].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[5].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[5].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[5].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[5].U16[7], s2value, 0, 1, 0, 1)
	}


_04001678:	// 4001678: <4b53294f>	VMADH     vec05 = ( acc+= (vec05 * vec19[2]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[5];
		VMADH_operation(0, state.v[5].U16[0], state.v[5].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[5].U16[1], state.v[5].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[5].U16[2], state.v[5].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[5].U16[3], state.v[5].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[5].U16[4], state.v[5].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[5].U16[5], state.v[5].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[5].U16[6], state.v[5].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[5].U16[7], state.v[5].U16[7], s2value, 1, 0, 0, 1)
	}


_0400167C:	// 400167c: <4a012950>	VADD      vec05 = vec05 + vec01[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[5].U16[0], state.v[5].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[5].U16[1], state.v[5].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[5].U16[2], state.v[5].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[5].U16[3], state.v[5].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[5].U16[4], state.v[5].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[5].U16[5], state.v[5].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[5].U16[6], state.v[5].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[5].U16[7], state.v[5].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001680:	// 4001680: <4a17b190>	VADD      vec06 = vec22 + vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[22].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[22].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[22].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[22].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[22].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[22].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[22].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[22].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001684:	// 4001684: <4a063190>	VADD      vec06 = vec06 + vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001688:	// 4001688: <4a003190>	VADD      vec06 = vec06 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400168C:	// 400168c: <4a023191>	VSUB      vec06 = vec06 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001690:	// 4001690: <4a17b1d1>	VSUB      vec07 = vec22 - vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[7].U16[0], state.v[22].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[7].U16[1], state.v[22].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[7].U16[2], state.v[22].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[7].U16[3], state.v[22].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[7].U16[4], state.v[22].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[7].U16[5], state.v[22].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[7].U16[6], state.v[22].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[7].U16[7], state.v[22].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001694:	// 4001694: <4bb33c45>	VMUDM     vec17 = ( acc = vec07 * vec19[5] ) >> 16
	{
		_u16		s2value = state.v[19].U16[2];
		VMUDM_operation(0, state.v[17].U16[0], state.v[7].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[7].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[7].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[7].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[7].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[7].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[7].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[7].U16[7], s2value, 0, 1, 0, 1)
	}


_04001698:	// 4001698: <4b9339cf>	VMADH     vec07 = ( acc+= (vec07 * vec19[4]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[3];
		VMADH_operation(0, state.v[7].U16[0], state.v[7].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[7].U16[1], state.v[7].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[7].U16[2], state.v[7].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[7].U16[3], state.v[7].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[7].U16[4], state.v[7].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[7].U16[5], state.v[7].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[7].U16[6], state.v[7].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[7].U16[7], state.v[7].U16[7], s2value, 1, 0, 0, 1)
	}


_0400169C:	// 400169c: <4a0039d0>	VADD      vec07 = vec07 + vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016A0:	// 40016a0: <4a008011>	VSUB      vec00 = vec16 - vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[0].U16[0], state.v[16].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[0].U16[1], state.v[16].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[0].U16[2], state.v[16].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[0].U16[3], state.v[16].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[0].U16[4], state.v[16].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[0].U16[5], state.v[16].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[0].U16[6], state.v[16].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[0].U16[7], state.v[16].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016A4:	// 40016a4: <4a0139d0>	VADD      vec07 = vec07 + vec01[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016A8:	// 40016a8: <e9600870>	SSV       [T3 + 0x00] = vec00 <00>
	Save16_DMEM(state.v[0].U16[7], (0xFFFFFFE0 + state.r[T3]) & 0xfff);


_040016AC:	// 40016ac: <4a0339d0>	VADD      vec07 = vec07 + vec03[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016B0:	// 40016b0: <4a008011>	VSUB      vec00 = vec16 - vec00[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[0].U16[0], state.v[16].U16[0], state.v[0].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[0].U16[1], state.v[16].U16[1], state.v[0].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[0].U16[2], state.v[16].U16[2], state.v[0].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[0].U16[3], state.v[16].U16[3], state.v[0].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[0].U16[4], state.v[16].U16[4], state.v[0].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[0].U16[5], state.v[16].U16[5], state.v[0].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[0].U16[6], state.v[16].U16[6], state.v[0].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[0].U16[7], state.v[16].U16[7], state.v[0].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016B4:	// 40016b4: <4a19c210>	VADD      vec08 = vec24 + vec25[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[8].U16[0], state.v[24].U16[0], state.v[25].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[8].U16[1], state.v[24].U16[1], state.v[25].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[8].U16[2], state.v[24].U16[2], state.v[25].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[8].U16[3], state.v[24].U16[3], state.v[25].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[8].U16[4], state.v[24].U16[4], state.v[25].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[8].U16[5], state.v[24].U16[5], state.v[25].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[8].U16[6], state.v[24].U16[6], state.v[25].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[8].U16[7], state.v[24].U16[7], state.v[25].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016B8:	// 40016b8: <4a19c251>	VSUB      vec09 = vec24 - vec25[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[24].U16[0], state.v[25].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[24].U16[1], state.v[25].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[24].U16[2], state.v[25].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[24].U16[3], state.v[25].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[24].U16[4], state.v[25].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[24].U16[5], state.v[25].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[24].U16[6], state.v[25].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[24].U16[7], state.v[25].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016BC:	// 40016bc: <4b734c45>	VMUDM     vec17 = ( acc = vec09 * vec19[3] ) >> 16
	{
		_u16		s2value = state.v[19].U16[4];
		VMUDM_operation(0, state.v[17].U16[0], state.v[9].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[9].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[9].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[9].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[9].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[9].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[9].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[9].U16[7], s2value, 0, 1, 0, 1)
	}


_040016C0:	// 40016c0: <4b534a4f>	VMADH     vec09 = ( acc+= (vec09 * vec19[2]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[5];
		VMADH_operation(0, state.v[9].U16[0], state.v[9].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[9].U16[1], state.v[9].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[9].U16[2], state.v[9].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[9].U16[3], state.v[9].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[9].U16[4], state.v[9].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[9].U16[5], state.v[9].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[9].U16[6], state.v[9].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[9].U16[7], state.v[9].U16[7], s2value, 1, 0, 0, 1)
	}


_040016C4:	// 40016c4: <4a1bd290>	VADD      vec10 = vec26 + vec27[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[26].U16[0], state.v[27].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[26].U16[1], state.v[27].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[26].U16[2], state.v[27].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[26].U16[3], state.v[27].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[26].U16[4], state.v[27].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[26].U16[5], state.v[27].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[26].U16[6], state.v[27].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[26].U16[7], state.v[27].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016C8:	// 40016c8: <4a0a5290>	VADD      vec10 = vec10 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016CC:	// 40016cc: <4a085290>	VADD      vec10 = vec10 + vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016D0:	// 40016d0: <4a1bd2d1>	VSUB      vec11 = vec26 - vec27[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[26].U16[0], state.v[27].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[26].U16[1], state.v[27].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[26].U16[2], state.v[27].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[26].U16[3], state.v[27].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[26].U16[4], state.v[27].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[26].U16[5], state.v[27].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[26].U16[6], state.v[27].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[26].U16[7], state.v[27].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016D4:	// 40016d4: <4bb35c45>	VMUDM     vec17 = ( acc = vec11 * vec19[5] ) >> 16
	{
		_u16		s2value = state.v[19].U16[2];
		VMUDM_operation(0, state.v[17].U16[0], state.v[11].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[11].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[11].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[11].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[11].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[11].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[11].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[11].U16[7], s2value, 0, 1, 0, 1)
	}


_040016D8:	// 40016d8: <4b935acf>	VMADH     vec11 = ( acc+= (vec11 * vec19[4]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[3];
		VMADH_operation(0, state.v[11].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[11].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[11].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[11].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[11].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[11].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[11].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[11].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
	}


_040016DC:	// 40016dc: <4a085ad0>	VADD      vec11 = vec11 + vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016E0:	// 40016e0: <4a095ad0>	VADD      vec11 = vec11 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016E4:	// 40016e4: <4a1de310>	VADD      vec12 = vec28 + vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[28].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[28].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[28].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[28].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[28].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[28].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[28].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[28].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016E8:	// 40016e8: <4a0c6310>	VADD      vec12 = vec12 + vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[12].U16[0], state.v[12].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[12].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[12].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[12].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[12].U16[4], state.v[12].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[12].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[12].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[12].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016EC:	// 40016ec: <4a0c2311>	VSUB      vec12 = vec04 - vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[12].U16[0], state.v[4].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[12].U16[1], state.v[4].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[12].U16[2], state.v[4].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[12].U16[3], state.v[4].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[12].U16[4], state.v[4].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[12].U16[5], state.v[4].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[12].U16[6], state.v[4].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[12].U16[7], state.v[4].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016F0:	// 40016f0: <4a1de351>	VSUB      vec13 = vec28 - vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[28].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[28].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[28].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[28].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[28].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[28].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[28].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[28].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040016F4:	// 40016f4: <4bb36c45>	VMUDM     vec17 = ( acc = vec13 * vec19[5] ) >> 16
	{
		_u16		s2value = state.v[19].U16[2];
		VMUDM_operation(0, state.v[17].U16[0], state.v[13].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[13].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[13].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[13].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[13].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[13].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[13].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[13].U16[7], s2value, 0, 1, 0, 1)
	}


_040016F8:	// 40016f8: <e94c0810>	SSV       [T2 + 0x00] = vec12 <00>
	Save16_DMEM(state.v[12].U16[7], (0x00000020 + state.r[T2]) & 0xfff);


_040016FC:	// 40016fc: <4b936b4f>	VMADH     vec13 = ( acc+= (vec13 * vec19[4]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[3];
		VMADH_operation(0, state.v[13].U16[0], state.v[13].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[13].U16[1], state.v[13].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[13].U16[2], state.v[13].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[13].U16[3], state.v[13].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[13].U16[4], state.v[13].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[13].U16[5], state.v[13].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[13].U16[6], state.v[13].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[13].U16[7], state.v[13].U16[7], s2value, 1, 0, 0, 1)
	}


_04001700:	// 4001700: <4a0c6b51>	VSUB      vec13 = vec13 - vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001704:	// 4001704: <4a056b51>	VSUB      vec13 = vec13 - vec05[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001708:	// 4001708: <4a1ff390>	VADD      vec14 = vec30 + vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[30].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[30].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[30].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[30].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[30].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[30].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[30].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[30].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400170C:	// 400170c: <4a0e7390>	VADD      vec14 = vec14 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001710:	// 4001710: <4a0e7390>	VADD      vec14 = vec14 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001714:	// 4001714: <4a0e3391>	VSUB      vec14 = vec06 - vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[14].U16[0], state.v[6].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[14].U16[1], state.v[6].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[14].U16[2], state.v[6].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[14].U16[3], state.v[6].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[14].U16[4], state.v[6].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[14].U16[5], state.v[6].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[14].U16[6], state.v[6].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[14].U16[7], state.v[6].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001718:	// 4001718: <4a1ff3d1>	VSUB      vec15 = vec30 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[15].U16[0], state.v[30].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[15].U16[1], state.v[30].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[15].U16[2], state.v[30].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[15].U16[3], state.v[30].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[15].U16[4], state.v[30].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[15].U16[5], state.v[30].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[15].U16[6], state.v[30].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[15].U16[7], state.v[30].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400171C:	// 400171c: <4bf37c45>	VMUDM     vec17 = ( acc = vec15 * vec19[7] ) >> 16
	{
		_u16		s2value = state.v[19].U16[0];
		VMUDM_operation(0, state.v[17].U16[0], state.v[15].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[17].U16[1], state.v[15].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[17].U16[2], state.v[15].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[17].U16[3], state.v[15].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[17].U16[4], state.v[15].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[17].U16[5], state.v[15].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[17].U16[6], state.v[15].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[17].U16[7], state.v[15].U16[7], s2value, 0, 1, 0, 1)
	}


_04001720:	// 4001720: <e9ae0810>	SSV       [T5 + 0x00] = vec14 <00>
	Save16_DMEM(state.v[14].U16[7], (0x00000020 + state.r[T5]) & 0xfff);


_04001724:	// 4001724: <4bd37bcf>	VMADH     vec15 = ( acc+= (vec15 * vec19[6]) << 16) >> 16
	{
		_u16		s2value = state.v[19].U16[1];
		VMADH_operation(0, state.v[15].U16[0], state.v[15].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[15].U16[1], state.v[15].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[15].U16[2], state.v[15].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[15].U16[3], state.v[15].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[15].U16[4], state.v[15].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[15].U16[5], state.v[15].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[15].U16[6], state.v[15].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[15].U16[7], state.v[15].U16[7], s2value, 1, 0, 0, 1)
	}


_04001728:	// 4001728: <4a077bd1>	VSUB      vec15 = vec15 - vec07[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[15].U16[0], state.v[15].U16[0], state.v[7].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[15].U16[1], state.v[15].U16[1], state.v[7].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[15].U16[2], state.v[15].U16[2], state.v[7].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[15].U16[3], state.v[15].U16[3], state.v[7].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[15].U16[4], state.v[15].U16[4], state.v[7].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[15].U16[5], state.v[15].U16[5], state.v[7].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[15].U16[6], state.v[15].U16[6], state.v[7].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[15].U16[7], state.v[15].U16[7], state.v[7].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400172C:	// 400172c: <4a017390>	VADD      vec14 = vec14 + vec01[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[14].U16[0], state.v[14].U16[0], state.v[1].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[14].U16[1], state.v[1].U16[1], 0, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[14].U16[2], state.v[1].U16[2], 0, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[14].U16[3], state.v[1].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[14].U16[4], state.v[14].U16[4], state.v[1].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[14].U16[5], state.v[1].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[14].U16[6], state.v[1].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[14].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001730:	// 4001730: <e9ce0810>	SSV       [T6 + 0x00] = vec14 <00>
	Save16_DMEM(state.v[14].U16[7], (0x00000020 + state.r[T6]) & 0xfff);


_04001734:	// 4001734: <e92f0870>	SSV       [T1 + 0x00] = vec15 <00>
	Save16_DMEM(state.v[15].U16[7], (0xFFFFFFE0 + state.r[T1]) & 0xfff);


_04001738:	// 4001738: <4a0a4a50>	VADD      vec09 = vec09 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[9].U16[0], state.v[9].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[9].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[9].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[9].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[9].U16[4], state.v[9].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[9].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[9].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[9].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400173C:	// 400173c: <4a060850>	VADD      vec01 = vec01 + vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[1].U16[0], state.v[1].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[1].U16[1], state.v[1].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[1].U16[2], state.v[1].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[1].U16[3], state.v[1].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[1].U16[4], state.v[1].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[1].U16[5], state.v[1].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[1].U16[6], state.v[1].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[1].U16[7], state.v[1].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001740:	// 4001740: <4a065191>	VSUB      vec06 = vec10 - vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[6].U16[0], state.v[10].U16[0], state.v[6].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[6].U16[1], state.v[10].U16[1], state.v[6].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[6].U16[2], state.v[10].U16[2], state.v[6].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[6].U16[3], state.v[10].U16[3], state.v[6].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[6].U16[4], state.v[10].U16[4], state.v[6].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[6].U16[5], state.v[10].U16[5], state.v[6].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[6].U16[6], state.v[10].U16[6], state.v[6].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[6].U16[7], state.v[10].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001744:	// 4001744: <4a014851>	VSUB      vec01 = vec09 - vec01[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[1].U16[0], state.v[9].U16[0], state.v[1].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[1].U16[1], state.v[9].U16[1], state.v[1].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[1].U16[2], state.v[9].U16[2], state.v[1].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[1].U16[3], state.v[9].U16[3], state.v[1].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[1].U16[4], state.v[9].U16[4], state.v[1].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[1].U16[5], state.v[9].U16[5], state.v[1].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[1].U16[6], state.v[9].U16[6], state.v[1].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[1].U16[7], state.v[9].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001748:	// 4001748: <e9a60830>	SSV       [T5 + 0x00] = vec06 <00>
	Save16_DMEM(state.v[6].U16[7], (0x00000060 + state.r[T5]) & 0xfff);


_0400174C:	// 400174c: <4a025290>	VADD      vec10 = vec10 + vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[10].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[10].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[10].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[10].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[10].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[10].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[10].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[10].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001750:	// 4001750: <4a0a2291>	VSUB      vec10 = vec04 - vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[10].U16[0], state.v[4].U16[0], state.v[10].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[10].U16[1], state.v[4].U16[1], state.v[10].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[10].U16[2], state.v[4].U16[2], state.v[10].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[10].U16[3], state.v[4].U16[3], state.v[10].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[10].U16[4], state.v[4].U16[4], state.v[10].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[10].U16[5], state.v[4].U16[5], state.v[10].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[10].U16[6], state.v[4].U16[6], state.v[10].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[10].U16[7], state.v[4].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001754:	// 4001754: <e94a0850>	SSV       [T2 + 0x00] = vec10 <00>
	Save16_DMEM(state.v[10].U16[7], (0xFFFFFFA0 + state.r[T2]) & 0xfff);


_04001758:	// 4001758: <4a0c1311>	VSUB      vec12 = vec02 - vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[12].U16[0], state.v[2].U16[0], state.v[12].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[12].U16[1], state.v[2].U16[1], state.v[12].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[12].U16[2], state.v[2].U16[2], state.v[12].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[12].U16[3], state.v[2].U16[3], state.v[12].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[12].U16[4], state.v[2].U16[4], state.v[12].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[12].U16[5], state.v[2].U16[5], state.v[12].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[12].U16[6], state.v[2].U16[6], state.v[12].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[12].U16[7], state.v[2].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400175C:	// 400175c: <e94c0870>	SSV       [T2 + 0x00] = vec12 <00>
	Save16_DMEM(state.v[12].U16[7], (0xFFFFFFE0 + state.r[T2]) & 0xfff);


_04001760:	// 4001760: <4a052150>	VADD      vec05 = vec04 + vec05[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[5].U16[0], state.v[4].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[5].U16[1], state.v[4].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[5].U16[2], state.v[4].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[5].U16[3], state.v[4].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[5].U16[4], state.v[4].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[5].U16[5], state.v[4].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[5].U16[6], state.v[4].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[5].U16[7], state.v[4].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001764:	// 4001764: <4a044111>	VSUB      vec04 = vec08 - vec04[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[4].U16[0], state.v[8].U16[0], state.v[4].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[4].U16[1], state.v[8].U16[1], state.v[4].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[4].U16[2], state.v[8].U16[2], state.v[4].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[4].U16[3], state.v[8].U16[3], state.v[4].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[4].U16[4], state.v[8].U16[4], state.v[4].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[4].U16[5], state.v[8].U16[5], state.v[4].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[4].U16[6], state.v[8].U16[6], state.v[4].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[4].U16[7], state.v[8].U16[7], state.v[4].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001768:	// 4001768: <e9440830>	SSV       [T2 + 0x00] = vec04 <00>
	Save16_DMEM(state.v[4].U16[7], (0x00000060 + state.r[T2]) & 0xfff);


_0400176C:	// 400176c: <4a080011>	VSUB      vec00 = vec00 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[8].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[8].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[8].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[8].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[8].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[8].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[8].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001770:	// 4001770: <e9600850>	SSV       [T3 + 0x00] = vec00 <00>
	Save16_DMEM(state.v[0].U16[7], (0xFFFFFFA0 + state.r[T3]) & 0xfff);


_04001774:	// 4001774: <4a0b39d1>	VSUB      vec07 = vec07 - vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[7].U16[0], state.v[7].U16[0], state.v[11].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[7].U16[1], state.v[7].U16[1], state.v[11].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[7].U16[2], state.v[7].U16[2], state.v[11].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[7].U16[3], state.v[7].U16[3], state.v[11].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[7].U16[4], state.v[7].U16[4], state.v[11].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[7].U16[5], state.v[7].U16[5], state.v[11].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[7].U16[6], state.v[7].U16[6], state.v[11].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[7].U16[7], state.v[7].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001778:	// 4001778: <e9270850>	SSV       [T1 + 0x00] = vec07 <00>
	Save16_DMEM(state.v[7].U16[7], (0xFFFFFFA0 + state.r[T1]) & 0xfff);


_0400177C:	// 400177c: <4a035ad1>	VSUB      vec11 = vec11 - vec03[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001780:	// 4001780: <e9c10830>	SSV       [T6 + 0x00] = vec01 <00>
	Save16_DMEM(state.v[1].U16[7], (0x00000060 + state.r[T6]) & 0xfff);


_04001784:	// 4001784: <4a055ad1>	VSUB      vec11 = vec11 - vec05[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[11].U16[0], state.v[11].U16[0], state.v[5].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[11].U16[1], state.v[11].U16[1], state.v[5].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[11].U16[2], state.v[11].U16[2], state.v[5].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[11].U16[3], state.v[11].U16[3], state.v[5].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[11].U16[4], state.v[11].U16[4], state.v[5].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[11].U16[5], state.v[11].U16[5], state.v[5].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[11].U16[6], state.v[11].U16[6], state.v[5].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[11].U16[7], state.v[11].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001788:	// 4001788: <e90b0830>	SSV       [T0 + 0x00] = vec11 <00>
	Save16_DMEM(state.v[11].U16[7], (0x00000060 + state.r[T0]) & 0xfff);


_0400178C:	// 400178c: <4a0d18d1>	VSUB      vec03 = vec03 - vec13[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[3].U16[0], state.v[3].U16[0], state.v[13].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[3].U16[1], state.v[3].U16[1], state.v[13].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[3].U16[2], state.v[3].U16[2], state.v[13].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[3].U16[3], state.v[3].U16[3], state.v[13].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[3].U16[4], state.v[3].U16[4], state.v[13].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[3].U16[5], state.v[3].U16[5], state.v[13].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[3].U16[6], state.v[3].U16[6], state.v[13].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[3].U16[7], state.v[3].U16[7], state.v[13].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001790:	// 4001790: <e9030810>	SSV       [T0 + 0x00] = vec03 <00>
	Save16_DMEM(state.v[3].U16[7], (0x00000020 + state.r[T0]) & 0xfff);


_04001794:	// 4001794: <4a026b50>	VADD      vec13 = vec13 + vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[13].U16[0], state.v[13].U16[0], state.v[2].U16[0], 0, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[13].U16[1], state.v[2].U16[1], 0, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[13].U16[2], state.v[2].U16[2], 0, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[13].U16[3], state.v[2].U16[3], 0, 0, 0, 1)
		VADD_operation(4, state.v[13].U16[4], state.v[13].U16[4], state.v[2].U16[4], 0, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[13].U16[5], state.v[2].U16[5], 0, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[13].U16[6], state.v[2].U16[6], 0, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[13].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001798:	// 4001798: <e90d0870>	SSV       [T0 + 0x00] = vec13 <00>
	Save16_DMEM(state.v[13].U16[7], (0xFFFFFFE0 + state.r[T0]) & 0xfff);


_0400179C:	// 400179c: <4a022891>	VSUB      vec02 = vec05 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[2].U16[0], state.v[5].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[2].U16[1], state.v[5].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[2].U16[2], state.v[5].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[2].U16[3], state.v[5].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[2].U16[4], state.v[5].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[2].U16[5], state.v[5].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[2].U16[6], state.v[5].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[2].U16[7], state.v[5].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040017A0:	// 40017a0: <4a091091>	VSUB      vec02 = vec02 - vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[9].U16[0], 0, 0, 0, 1)
		VSUB_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[9].U16[1], 0, 0, 0, 1)
		VSUB_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[9].U16[2], 0, 0, 0, 1)
		VSUB_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[9].U16[3], 0, 0, 0, 1)
		VSUB_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[9].U16[4], 0, 0, 0, 1)
		VSUB_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[9].U16[5], 0, 0, 0, 1)
		VSUB_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[9].U16[6], 0, 0, 0, 1)
		VSUB_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040017A4:	// 40017a4: <e9020850>	SSV       [T0 + 0x00] = vec02 <00>
	Save16_DMEM(state.v[2].U16[7], (0xFFFFFFA0 + state.r[T0]) & 0xfff);


_040017A8:	// 40017a8: <31c9ffe0>	ANDI      T1 = T6 & ffe0
	state.r[T1] = (state.r[T6] & 0x0000FFE0);


_040017AC:	// 40017ac: <200a0080>	ADDI      T2 = R0 + 0080
	state.r[T2] = 0 + 0x0080;


_040017B0:	// 40017b0: <014c5022>	SUB       T2 = T2-T4
	state.r[T2] = (state.r[T2] - state.r[T4]);


_040017B4:	// 40017b4: <c9222000>	LQV       vec02 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040017B8:	// 40017b8: <c9412000>	LQV       vec01 <00> = [T2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_040017BC:	// 40017bc: <c9412801>	LRV       vec01 <00> = [T2 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[1].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040017C0:	// 40017c0: <c9242001>	LQV       vec04 <00> = [T1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_040017C4:	// 40017c4: <c9432001>	LQV       vec03 <00> = [T2 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_040017C8:	// 40017c8: <c9432802>	LRV       vec03 <00> = [T2 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[3].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040017CC:	// 40017cc: <c9262002>	LQV       vec06 <00> = [T1 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[6].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[6].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[6].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[6].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[6].U64[0] = state.v[6].U64[0] & ~temp1;
			state.v[6].U64[1] = state.v[6].U64[1] & ~temp2;
			state.v[6].U64[0] = state.v[6].U64[0] | value1;
			state.v[6].U64[1] = state.v[6].U64[1] | value2;
		}
	}


_040017D0:	// 40017d0: <c9452004>	LQV       vec05 <00> = [T2 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[5].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[5].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[5].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[5].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[5].U64[0] = state.v[5].U64[0] & ~temp1;
			state.v[5].U64[1] = state.v[5].U64[1] & ~temp2;
			state.v[5].U64[0] = state.v[5].U64[0] | value1;
			state.v[5].U64[1] = state.v[5].U64[1] | value2;
		}
	}


_040017D4:	// 40017d4: <c9452805>	LRV       vec05 <00> = [T2 + 0x50]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000050 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[5].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040017D8:	// 40017d8: <c9282003>	LQV       vec08 <00> = [T1 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_040017DC:	// 40017dc: <c9472005>	LQV       vec07 <00> = [T2 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[7].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[7].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[7].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[7].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[7].U64[0] = state.v[7].U64[0] & ~temp1;
			state.v[7].U64[1] = state.v[7].U64[1] & ~temp2;
			state.v[7].U64[0] = state.v[7].U64[0] | value1;
			state.v[7].U64[1] = state.v[7].U64[1] | value2;
		}
	}


_040017E0:	// 40017e0: <c9472806>	LRV       vec07 <00> = [T2 + 0x60]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000060 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[7].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040017E4:	// 40017e4: <4a011080>	VMULF     vec02 = ( acc = (vec02 * vec01[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
	}


_040017E8:	// 40017e8: <200b0ce8>	ADDI      T3 = R0 + 0ce8
	state.r[T3] = 0 + 0x0CE8;


_040017EC:	// 40017ec: <4a032100>	VMULF     vec04 = ( acc = (vec04 * vec03[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
	}


_040017F0:	// 40017f0: <c9741800>	LDV       vec20 <00> = [T3 + 0x00]
	rsp_ldv_compiler((state.r[T3] & 0xFFF), 20, 8);


_040017F4:	// 40017f4: <4a053180>	VMULF     vec06 = ( acc = (vec06 * vec05[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
	}


_040017F8:	// 40017f8: <200b0008>	ADDI      T3 = R0 + 0008
	state.r[T3] = 0 + 0x0008;


_040017FC:	// 40017fc: <4a074200>	VMULF     vec08 = ( acc = (vec08 * vec07[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[8].U16[0], state.v[8].U16[0], state.v[7].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[8].U16[1], state.v[8].U16[1], state.v[7].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[8].U16[2], state.v[8].U16[2], state.v[7].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[8].U16[3], state.v[8].U16[3], state.v[7].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[8].U16[4], state.v[8].U16[4], state.v[7].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[8].U16[5], state.v[8].U16[5], state.v[7].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[8].U16[6], state.v[8].U16[6], state.v[7].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[8].U16[7], state.v[8].U16[7], state.v[7].U16[7], 1, 0, 0, 1)
	}


_04001800:	// 4001800: <214a0080>	ADDI      T2 = T2 + 0080
	state.r[T2] = state.r[T2] + 0x0080;


_04001804:	// 4001804: <4ac21250>	VADD      vec09 = vec02 + vec02[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[2].U16[1];
		VADD_operation(0, state.v[9].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[2].U16[5];
		VADD_operation(4, state.v[9].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001808:	// 4001808: <21290040>	ADDI      T1 = T1 + 0040
	state.r[T1] = state.r[T1] + 0x0040;


_0400180C:	// 400180c: <4ac42290>	VADD      vec10 = vec04 + vec04[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[4].U16[1];
		VADD_operation(0, state.v[10].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[4].U16[5];
		VADD_operation(4, state.v[10].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001810:	// 4001810: <c9412000>	LQV       vec01 <00> = [T2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_04001814:	// 4001814: <4ac632d0>	VADD      vec11 = vec06 + vec06[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[6].U16[1];
		VADD_operation(0, state.v[11].U16[0], state.v[6].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[6].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[6].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[6].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[6].U16[5];
		VADD_operation(4, state.v[11].U16[4], state.v[6].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[6].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[6].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[6].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001818:	// 4001818: <c9412801>	LRV       vec01 <00> = [T2 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[1].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400181C:	// 400181c: <4ac84310>	VADD      vec12 = vec08 + vec08[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[8].U16[1];
		VADD_operation(0, state.v[12].U16[0], state.v[8].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[8].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[8].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[8].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[8].U16[5];
		VADD_operation(4, state.v[12].U16[4], state.v[8].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[8].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[8].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[8].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001820:	// 4001820: <c9432001>	LQV       vec03 <00> = [T2 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_04001824:	// 4001824: <4ae21350>	VADD      vec13 = vec02 + vec02[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[2].U16[0];
		VADD_operation(0, state.v[13].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[2].U16[4];
		VADD_operation(4, state.v[13].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001828:	// 4001828: <c9432802>	LRV       vec03 <00> = [T2 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[3].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400182C:	// 400182c: <4ae42390>	VADD      vec14 = vec04 + vec04[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[4].U16[0];
		VADD_operation(0, state.v[14].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[4].U16[4];
		VADD_operation(4, state.v[14].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001830:	// 4001830: <c9452004>	LQV       vec05 <00> = [T2 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[5].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[5].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[5].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[5].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[5].U64[0] = state.v[5].U64[0] & ~temp1;
			state.v[5].U64[1] = state.v[5].U64[1] & ~temp2;
			state.v[5].U64[0] = state.v[5].U64[0] | value1;
			state.v[5].U64[1] = state.v[5].U64[1] | value2;
		}
	}


_04001834:	// 4001834: <4ae633d0>	VADD      vec15 = vec06 + vec06[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[6].U16[0];
		VADD_operation(0, state.v[15].U16[0], state.v[6].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[15].U16[1], state.v[6].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[15].U16[2], state.v[6].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[15].U16[3], state.v[6].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[6].U16[4];
		VADD_operation(4, state.v[15].U16[4], state.v[6].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[15].U16[5], state.v[6].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[15].U16[6], state.v[6].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[15].U16[7], state.v[6].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001838:	// 4001838: <c9452805>	LRV       vec05 <00> = [T2 + 0x50]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000050 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[5].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400183C:	// 400183c: <4ae84410>	VADD      vec16 = vec08 + vec08[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[8].U16[0];
		VADD_operation(0, state.v[16].U16[0], state.v[8].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[8].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[8].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[8].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[8].U16[4];
		VADD_operation(4, state.v[16].U16[4], state.v[8].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[8].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[8].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[8].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001840:	// 4001840: <c9472005>	LQV       vec07 <00> = [T2 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[7].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[7].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[7].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[7].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[7].U64[0] = state.v[7].U64[0] & ~temp1;
			state.v[7].U64[1] = state.v[7].U64[1] & ~temp2;
			state.v[7].U64[0] = state.v[7].U64[0] | value1;
			state.v[7].U64[1] = state.v[7].U64[1] | value2;
		}
	}


_04001844:	// 4001844: <4aad4810>	VADD      vec00 = vec09 + vec13[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[13].U16[2];
		VADD_operation(0, state.v[0].U16[0], state.v[9].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[9].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[9].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[9].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[13].U16[6];
		VADD_operation(4, state.v[0].U16[4], state.v[9].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[9].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[9].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[9].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001848:	// 4001848: <c9472806>	LRV       vec07 <00> = [T2 + 0x60]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000060 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[7].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_0400184C:	// 400184c: <4aae5450>	VADD      vec17 = vec10 + vec14[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[14].U16[2];
		VADD_operation(0, state.v[17].U16[0], state.v[10].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[10].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[10].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[10].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[14].U16[6];
		VADD_operation(4, state.v[17].U16[4], state.v[10].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[10].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[10].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[10].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001850:	// 4001850: <c9222000>	LQV       vec02 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_04001854:	// 4001854: <4aaf5c90>	VADD      vec18 = vec11 + vec15[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[15].U16[2];
		VADD_operation(0, state.v[18].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[15].U16[6];
		VADD_operation(4, state.v[18].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001858:	// 4001858: <c9242001>	LQV       vec04 <00> = [T1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_0400185C:	// 400185c: <4ab064d0>	VADD      vec19 = vec12 + vec16[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[16].U16[2];
		VADD_operation(0, state.v[19].U16[0], state.v[12].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[19].U16[1], state.v[12].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[19].U16[2], state.v[12].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[19].U16[3], state.v[12].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[16].U16[6];
		VADD_operation(4, state.v[19].U16[4], state.v[12].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[19].U16[5], state.v[12].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[19].U16[6], state.v[12].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[19].U16[7], state.v[12].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001860:	// 4001860: <c9262002>	LQV       vec06 <00> = [T1 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[6].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[6].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[6].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[6].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[6].U64[0] = state.v[6].U64[0] & ~temp1;
			state.v[6].U64[1] = state.v[6].U64[1] & ~temp2;
			state.v[6].U64[0] = state.v[6].U64[0] | value1;
			state.v[6].U64[1] = state.v[6].U64[1] | value2;
		}
	}


_04001864:	// 4001864: <4a110010>	VADD      vec00 = vec00 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001868:	// 4001868: <c9282003>	LQV       vec08 <00> = [T1 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_0400186C:	// 400186c: <4a139490>	VADD      vec18 = vec18 + vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001870:	// 4001870: <4a011080>	VMULF     vec02 = ( acc = (vec02 * vec01[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
	}


_04001874:	// 4001874: <4b800010>	VADD      vec00 = vec00 + vec00[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[0].U16[3];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001878:	// 4001878: <4a032100>	VMULF     vec04 = ( acc = (vec04 * vec03[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
	}


_0400187C:	// 400187c: <4b929490>	VADD      vec18 = vec18 + vec18[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[18].U16[3];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001880:	// 4001880: <4a053180>	VMULF     vec06 = ( acc = (vec06 * vec05[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
	}


_04001884:	// 4001884: <ea600800>	SSV       [S3 + 0x00] = vec00 <00>
	Save16_DMEM(state.v[0].U16[7], (0x00000000 + state.r[S3]) & 0xfff);


_04001888:	// 4001888: <4a074200>	VMULF     vec08 = ( acc = (vec08 * vec07[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[8].U16[0], state.v[8].U16[0], state.v[7].U16[0], 1, 1, 1, 1)
		VMULF_operation(1, state.v[8].U16[1], state.v[8].U16[1], state.v[7].U16[1], 1, 1, 1, 1)
		VMULF_operation(2, state.v[8].U16[2], state.v[8].U16[2], state.v[7].U16[2], 1, 1, 1, 1)
		VMULF_operation(3, state.v[8].U16[3], state.v[8].U16[3], state.v[7].U16[3], 1, 1, 1, 1)
		VMULF_operation(4, state.v[8].U16[4], state.v[8].U16[4], state.v[7].U16[4], 1, 1, 1, 1)
		VMULF_operation(5, state.v[8].U16[5], state.v[8].U16[5], state.v[7].U16[5], 1, 1, 1, 1)
		VMULF_operation(6, state.v[8].U16[6], state.v[8].U16[6], state.v[7].U16[6], 1, 1, 1, 1)
		VMULF_operation(7, state.v[8].U16[7], state.v[8].U16[7], state.v[7].U16[7], 1, 1, 1, 1)
	}


_0400188C:	// 400188c: <216bffff>	ADDI      T3 = T3 + ffff
	state.r[T3] = state.r[T3] + 0xFFFFFFFF;


_04001890:	// 4001890: <ea720801>	SSV       [S3 + 0x02] = vec18 <00>
	Save16_DMEM(state.v[18].U16[7], (0x00000002 + state.r[S3]) & 0xfff);


_04001894:	// 4001894: <1d60ffda>	BGTZ      (R0>0) --> 1800
	sp_reg_pc = 0x04001894;
	if((_s32)state.r[T3] > 0)
	{
		state.r[S3] = state.r[S3] + 0x0004;
		goto _04001800;
	}


_04001898:	// 4001898: <22730004>	ADDI      S3 = S3 + 0004
	state.r[S3] = state.r[S3] + 0x0004;


_0400189C:	// 400189c: <4ac21250>	VADD      vec09 = vec02 + vec02[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[2].U16[1];
		VADD_operation(0, state.v[9].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[2].U16[5];
		VADD_operation(4, state.v[9].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018A0:	// 40018a0: <31880002>	ANDI      T0 = T4 & 0002
	state.r[T0] = (state.r[T4] & 0x00000002);


_040018A4:	// 40018a4: <4ac42290>	VADD      vec10 = vec04 + vec04[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[4].U16[1];
		VADD_operation(0, state.v[10].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[4].U16[5];
		VADD_operation(4, state.v[10].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018A8:	// 40018a8: <4ae21350>	VADD      vec13 = vec02 + vec02[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[2].U16[0];
		VADD_operation(0, state.v[13].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[2].U16[4];
		VADD_operation(4, state.v[13].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018AC:	// 40018ac: <4ae42390>	VADD      vec14 = vec04 + vec04[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[4].U16[0];
		VADD_operation(0, state.v[14].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[4].U16[4];
		VADD_operation(4, state.v[14].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018B0:	// 40018b0: <4a0a4810>	VADD      vec00 = vec09 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[9].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[9].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[9].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[9].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[9].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[9].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[9].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[9].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018B4:	// 40018b4: <4b800010>	VADD      vec00 = vec00 + vec00[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[0].U16[3];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018B8:	// 40018b8: <4a0e6c50>	VADD      vec17 = vec13 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[13].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[13].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[13].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[13].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[13].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[13].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[13].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[13].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018BC:	// 40018bc: <2129ffc0>	ADDI      T1 = T1 + ffc0
	state.r[T1] = state.r[T1] + 0xFFFFFFC0;


_040018C0:	// 40018c0: <4bb18c50>	VADD      vec17 = vec17 + vec17[5]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[17].U16[2];
		VADD_operation(0, state.v[17].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040018C4:	// 40018c4: <4b340085>	VMUDM     vec02 = ( acc = vec00 * vec20[1] ) >> 16
	{
		_u16		s2value = state.v[20].U16[6];
		VMUDM_operation(0, state.v[2].U16[0], state.v[0].U16[0], s2value, 1, 1, 0, 1)
		VMUDM_operation(1, state.v[2].U16[1], state.v[0].U16[1], s2value, 1, 1, 0, 1)
		VMUDM_operation(2, state.v[2].U16[2], state.v[0].U16[2], s2value, 1, 1, 0, 1)
		VMUDM_operation(3, state.v[2].U16[3], state.v[0].U16[3], s2value, 1, 1, 0, 1)
		VMUDM_operation(4, state.v[2].U16[4], state.v[0].U16[4], s2value, 1, 1, 0, 1)
		VMUDM_operation(5, state.v[2].U16[5], state.v[0].U16[5], s2value, 1, 1, 0, 1)
		VMUDM_operation(6, state.v[2].U16[6], state.v[0].U16[6], s2value, 1, 1, 0, 1)
		VMUDM_operation(7, state.v[2].U16[7], state.v[0].U16[7], s2value, 1, 1, 0, 1)
	}


_040018C8:	// 40018c8: <4b14000f>	VMADH     vec00 = ( acc+= (vec00 * vec20[0]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMADH_operation(0, state.v[0].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[0].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[0].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[0].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[0].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[0].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[0].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[0].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
	}


_040018CC:	// 40018cc: <4b348905>	VMUDM     vec04 = ( acc = vec17 * vec20[1] ) >> 16
	{
		_u16		s2value = state.v[20].U16[6];
		VMUDM_operation(0, state.v[4].U16[0], state.v[17].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[4].U16[1], state.v[17].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[4].U16[2], state.v[17].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[4].U16[3], state.v[17].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[4].U16[4], state.v[17].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[4].U16[5], state.v[17].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[4].U16[6], state.v[17].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[4].U16[7], state.v[17].U16[7], s2value, 1, 1, 1, 1)
	}


_040018D0:	// 40018d0: <200b0ce8>	ADDI      T3 = R0 + 0ce8
	state.r[T3] = 0 + 0x0CE8;


_040018D4:	// 40018d4: <4b148c4f>	VMADH     vec17 = ( acc+= (vec17 * vec20[0]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMADH_operation(0, state.v[17].U16[0], state.v[17].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[17].U16[1], state.v[17].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[17].U16[2], state.v[17].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[17].U16[3], state.v[17].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[17].U16[4], state.v[17].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[17].U16[5], state.v[17].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[17].U16[6], state.v[17].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[17].U16[7], state.v[17].U16[7], s2value, 1, 1, 0, 1)
	}


_040018D8:	// 40018d8: <15000003>	BNE       (R0!=T0) --> 18e8
	sp_reg_pc = 0x040018D8;
	if (state.r[T0] != 0)
	{
		Save16_DMEM(state.v[0].U16[7], (0x00000000 + state.r[S3]) & 0xfff);
		goto _040018E8;
	}


_040018DC:	// 40018dc: <ea600800>	SSV       [S3 + 0x00] = vec00 <00>
	Save16_DMEM(state.v[0].U16[7], (0x00000000 + state.r[S3]) & 0xfff);


_040018E0:	// 40018e0: <c9741200>	LLV       vec20 <04> = [T3 + 0x00]
	state.v[20].U32[2] = Load32_DMEM(0x00000000 + state.r[T3]);


_040018E4:	// 40018e4: <ea710900>	SSV       [S3 + 0x00] = vec17 <02>
	Save16_DMEM(state.v[17].U16[6], (0x00000000 + state.r[S3]) & 0xfff);


_040018E8:	// 40018e8: <200a04be>	ADDI      T2 = R0 + 04be
	state.r[T2] = 0 + 0x04BE;


_040018EC:	// 40018ec: <014c5022>	SUB       T2 = T2-T4
	state.r[T2] = (state.r[T2] - state.r[T4]);


_040018F0:	// 40018f0: <c9222002>	LQV       vec02 <00> = [T1 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040018F4:	// 40018f4: <c9412000>	LQV       vec01 <00> = [T2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_040018F8:	// 40018f8: <c9412801>	LRV       vec01 <00> = [T2 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[1].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040018FC:	// 40018fc: <c9242003>	LQV       vec04 <00> = [T1 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_04001900:	// 4001900: <c9432001>	LQV       vec03 <00> = [T2 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_04001904:	// 4001904: <c9432802>	LRV       vec03 <00> = [T2 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[3].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001908:	// 4001908: <c9262000>	LQV       vec06 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[6].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[6].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[6].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[6].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[6].U64[0] = state.v[6].U64[0] & ~temp1;
			state.v[6].U64[1] = state.v[6].U64[1] & ~temp2;
			state.v[6].U64[0] = state.v[6].U64[0] | value1;
			state.v[6].U64[1] = state.v[6].U64[1] | value2;
		}
	}


_0400190C:	// 400190c: <c9452004>	LQV       vec05 <00> = [T2 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[5].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[5].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[5].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[5].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[5].U64[0] = state.v[5].U64[0] & ~temp1;
			state.v[5].U64[1] = state.v[5].U64[1] & ~temp2;
			state.v[5].U64[0] = state.v[5].U64[0] | value1;
			state.v[5].U64[1] = state.v[5].U64[1] | value2;
		}
	}


_04001910:	// 4001910: <c9452805>	LRV       vec05 <00> = [T2 + 0x50]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000050 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[5].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001914:	// 4001914: <c9282001>	LQV       vec08 <00> = [T1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_04001918:	// 4001918: <c9472005>	LQV       vec07 <00> = [T2 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[7].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[7].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[7].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[7].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[7].U64[0] = state.v[7].U64[0] & ~temp1;
			state.v[7].U64[1] = state.v[7].U64[1] & ~temp2;
			state.v[7].U64[0] = state.v[7].U64[0] | value1;
			state.v[7].U64[1] = state.v[7].U64[1] | value2;
		}
	}


_0400191C:	// 400191c: <c9472806>	LRV       vec07 <00> = [T2 + 0x60]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000060 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[7].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001920:	// 4001920: <4a011080>	VMULF     vec02 = ( acc = (vec02 * vec01[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
	}


_04001924:	// 4001924: <200b0008>	ADDI      T3 = R0 + 0008
	state.r[T3] = 0 + 0x0008;


_04001928:	// 4001928: <4a032100>	VMULF     vec04 = ( acc = (vec04 * vec03[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
	}


_0400192C:	// 400192c: <4a053180>	VMULF     vec06 = ( acc = (vec06 * vec05[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
	}


_04001930:	// 4001930: <4a074200>	VMULF     vec08 = ( acc = (vec08 * vec07[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[8].U16[0], state.v[8].U16[0], state.v[7].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[8].U16[1], state.v[8].U16[1], state.v[7].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[8].U16[2], state.v[8].U16[2], state.v[7].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[8].U16[3], state.v[8].U16[3], state.v[7].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[8].U16[4], state.v[8].U16[4], state.v[7].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[8].U16[5], state.v[8].U16[5], state.v[7].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[8].U16[6], state.v[8].U16[6], state.v[7].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[8].U16[7], state.v[8].U16[7], state.v[7].U16[7], 1, 0, 0, 1)
	}


_04001934:	// 4001934: <214a0080>	ADDI      T2 = T2 + 0080
	state.r[T2] = state.r[T2] + 0x0080;


_04001938:	// 4001938: <4ac21250>	VADD      vec09 = vec02 + vec02[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[2].U16[1];
		VADD_operation(0, state.v[9].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[2].U16[5];
		VADD_operation(4, state.v[9].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400193C:	// 400193c: <2129ffc0>	ADDI      T1 = T1 + ffc0
	state.r[T1] = state.r[T1] + 0xFFFFFFC0;


_04001940:	// 4001940: <4ac42290>	VADD      vec10 = vec04 + vec04[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[4].U16[1];
		VADD_operation(0, state.v[10].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[4].U16[5];
		VADD_operation(4, state.v[10].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001944:	// 4001944: <c9412000>	LQV       vec01 <00> = [T2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[1].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[1].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[1].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[1].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[1].U64[0] = state.v[1].U64[0] & ~temp1;
			state.v[1].U64[1] = state.v[1].U64[1] & ~temp2;
			state.v[1].U64[0] = state.v[1].U64[0] | value1;
			state.v[1].U64[1] = state.v[1].U64[1] | value2;
		}
	}


_04001948:	// 4001948: <4ac632d0>	VADD      vec11 = vec06 + vec06[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[6].U16[1];
		VADD_operation(0, state.v[11].U16[0], state.v[6].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[6].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[6].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[6].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[6].U16[5];
		VADD_operation(4, state.v[11].U16[4], state.v[6].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[6].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[6].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[6].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400194C:	// 400194c: <c9412801>	LRV       vec01 <00> = [T2 + 0x10]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000010 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[1].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001950:	// 4001950: <4ac84310>	VADD      vec12 = vec08 + vec08[2h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[8].U16[1];
		VADD_operation(0, state.v[12].U16[0], state.v[8].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[12].U16[1], state.v[8].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[12].U16[2], state.v[8].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[12].U16[3], state.v[8].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[8].U16[5];
		VADD_operation(4, state.v[12].U16[4], state.v[8].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[12].U16[5], state.v[8].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[12].U16[6], state.v[8].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[12].U16[7], state.v[8].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001954:	// 4001954: <c9432001>	LQV       vec03 <00> = [T2 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[3].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[3].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[3].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[3].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[3].U64[0] = state.v[3].U64[0] & ~temp1;
			state.v[3].U64[1] = state.v[3].U64[1] & ~temp2;
			state.v[3].U64[0] = state.v[3].U64[0] | value1;
			state.v[3].U64[1] = state.v[3].U64[1] | value2;
		}
	}


_04001958:	// 4001958: <4ae21350>	VADD      vec13 = vec02 + vec02[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[2].U16[0];
		VADD_operation(0, state.v[13].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[13].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[13].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[13].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[2].U16[4];
		VADD_operation(4, state.v[13].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[13].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[13].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[13].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400195C:	// 400195c: <c9432802>	LRV       vec03 <00> = [T2 + 0x20]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000020 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[3].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001960:	// 4001960: <4ae42390>	VADD      vec14 = vec04 + vec04[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[4].U16[0];
		VADD_operation(0, state.v[14].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[14].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[14].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[14].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[4].U16[4];
		VADD_operation(4, state.v[14].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[14].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[14].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[14].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001964:	// 4001964: <c9452004>	LQV       vec05 <00> = [T2 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[5].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[5].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[5].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[5].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[5].U64[0] = state.v[5].U64[0] & ~temp1;
			state.v[5].U64[1] = state.v[5].U64[1] & ~temp2;
			state.v[5].U64[0] = state.v[5].U64[0] | value1;
			state.v[5].U64[1] = state.v[5].U64[1] | value2;
		}
	}


_04001968:	// 4001968: <4ae633d0>	VADD      vec15 = vec06 + vec06[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[6].U16[0];
		VADD_operation(0, state.v[15].U16[0], state.v[6].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[15].U16[1], state.v[6].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[15].U16[2], state.v[6].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[15].U16[3], state.v[6].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[6].U16[4];
		VADD_operation(4, state.v[15].U16[4], state.v[6].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[15].U16[5], state.v[6].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[15].U16[6], state.v[6].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[15].U16[7], state.v[6].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400196C:	// 400196c: <c9452805>	LRV       vec05 <00> = [T2 + 0x50]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000050 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[5].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001970:	// 4001970: <4ae84410>	VADD      vec16 = vec08 + vec08[3h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[8].U16[0];
		VADD_operation(0, state.v[16].U16[0], state.v[8].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[8].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[8].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[8].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[8].U16[4];
		VADD_operation(4, state.v[16].U16[4], state.v[8].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[8].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[8].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[8].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001974:	// 4001974: <c9472005>	LQV       vec07 <00> = [T2 + 0x50]
	{
		_u32 addr = (0x00000050 + state.r[T2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[7].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[7].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[7].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[7].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[7].U64[0] = state.v[7].U64[0] & ~temp1;
			state.v[7].U64[1] = state.v[7].U64[1] & ~temp2;
			state.v[7].U64[0] = state.v[7].U64[0] | value1;
			state.v[7].U64[1] = state.v[7].U64[1] | value2;
		}
	}


_04001978:	// 4001978: <4aad4811>	VSUB      vec00 = vec09 - vec13[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[13].U16[2];
		VSUB_operation(0, state.v[0].U16[0], state.v[9].U16[0], s2value, 1, 0, 0, 1)
		VSUB_operation(1, state.v[0].U16[1], state.v[9].U16[1], s2value, 1, 0, 0, 1)
		VSUB_operation(2, state.v[0].U16[2], state.v[9].U16[2], s2value, 1, 0, 0, 1)
		VSUB_operation(3, state.v[0].U16[3], state.v[9].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[13].U16[6];
		VSUB_operation(4, state.v[0].U16[4], state.v[9].U16[4], s2value, 1, 0, 0, 1)
		VSUB_operation(5, state.v[0].U16[5], state.v[9].U16[5], s2value, 1, 0, 0, 1)
		VSUB_operation(6, state.v[0].U16[6], state.v[9].U16[6], s2value, 1, 0, 0, 1)
		VSUB_operation(7, state.v[0].U16[7], state.v[9].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400197C:	// 400197c: <c9472806>	LRV       vec07 <00> = [T2 + 0x60]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000060 + state.r[T2]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[7].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_04001980:	// 4001980: <4aae5451>	VSUB      vec17 = vec10 - vec14[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[14].U16[2];
		VSUB_operation(0, state.v[17].U16[0], state.v[10].U16[0], s2value, 1, 0, 0, 1)
		VSUB_operation(1, state.v[17].U16[1], state.v[10].U16[1], s2value, 1, 0, 0, 1)
		VSUB_operation(2, state.v[17].U16[2], state.v[10].U16[2], s2value, 1, 0, 0, 1)
		VSUB_operation(3, state.v[17].U16[3], state.v[10].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[14].U16[6];
		VSUB_operation(4, state.v[17].U16[4], state.v[10].U16[4], s2value, 1, 0, 0, 1)
		VSUB_operation(5, state.v[17].U16[5], state.v[10].U16[5], s2value, 1, 0, 0, 1)
		VSUB_operation(6, state.v[17].U16[6], state.v[10].U16[6], s2value, 1, 0, 0, 1)
		VSUB_operation(7, state.v[17].U16[7], state.v[10].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001984:	// 4001984: <c9222002>	LQV       vec02 <00> = [T1 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_04001988:	// 4001988: <4aaf5c91>	VSUB      vec18 = vec11 - vec15[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[15].U16[2];
		VSUB_operation(0, state.v[18].U16[0], state.v[11].U16[0], s2value, 1, 0, 0, 1)
		VSUB_operation(1, state.v[18].U16[1], state.v[11].U16[1], s2value, 1, 0, 0, 1)
		VSUB_operation(2, state.v[18].U16[2], state.v[11].U16[2], s2value, 1, 0, 0, 1)
		VSUB_operation(3, state.v[18].U16[3], state.v[11].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[15].U16[6];
		VSUB_operation(4, state.v[18].U16[4], state.v[11].U16[4], s2value, 1, 0, 0, 1)
		VSUB_operation(5, state.v[18].U16[5], state.v[11].U16[5], s2value, 1, 0, 0, 1)
		VSUB_operation(6, state.v[18].U16[6], state.v[11].U16[6], s2value, 1, 0, 0, 1)
		VSUB_operation(7, state.v[18].U16[7], state.v[11].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400198C:	// 400198c: <c9242003>	LQV       vec04 <00> = [T1 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_04001990:	// 4001990: <4ab064d1>	VSUB      vec19 = vec12 - vec16[1h]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value;
		s2value = state.v[16].U16[2];
		VSUB_operation(0, state.v[19].U16[0], state.v[12].U16[0], s2value, 1, 0, 0, 1)
		VSUB_operation(1, state.v[19].U16[1], state.v[12].U16[1], s2value, 1, 0, 0, 1)
		VSUB_operation(2, state.v[19].U16[2], state.v[12].U16[2], s2value, 1, 0, 0, 1)
		VSUB_operation(3, state.v[19].U16[3], state.v[12].U16[3], s2value, 1, 0, 0, 1)
		s2value = state.v[16].U16[6];
		VSUB_operation(4, state.v[19].U16[4], state.v[12].U16[4], s2value, 1, 0, 0, 1)
		VSUB_operation(5, state.v[19].U16[5], state.v[12].U16[5], s2value, 1, 0, 0, 1)
		VSUB_operation(6, state.v[19].U16[6], state.v[12].U16[6], s2value, 1, 0, 0, 1)
		VSUB_operation(7, state.v[19].U16[7], state.v[12].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001994:	// 4001994: <c9262000>	LQV       vec06 <00> = [T1 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[6].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[6].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[6].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[6].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[6].U64[0] = state.v[6].U64[0] & ~temp1;
			state.v[6].U64[1] = state.v[6].U64[1] & ~temp2;
			state.v[6].U64[0] = state.v[6].U64[0] | value1;
			state.v[6].U64[1] = state.v[6].U64[1] | value2;
		}
	}


_04001998:	// 4001998: <4a110010>	VADD      vec00 = vec00 + vec17[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], state.v[17].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], state.v[17].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], state.v[17].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], state.v[17].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], state.v[17].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], state.v[17].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], state.v[17].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], state.v[17].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_0400199C:	// 400199c: <c9282001>	LQV       vec08 <00> = [T1 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T1]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_040019A0:	// 40019a0: <4a139490>	VADD      vec18 = vec18 + vec19[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], state.v[19].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], state.v[19].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], state.v[19].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], state.v[19].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], state.v[19].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], state.v[19].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], state.v[19].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], state.v[19].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019A4:	// 40019a4: <4a011080>	VMULF     vec02 = ( acc = (vec02 * vec01[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[2].U16[0], state.v[2].U16[0], state.v[1].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[2].U16[1], state.v[2].U16[1], state.v[1].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[2].U16[2], state.v[2].U16[2], state.v[1].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[2].U16[3], state.v[2].U16[3], state.v[1].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[2].U16[4], state.v[2].U16[4], state.v[1].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[2].U16[5], state.v[2].U16[5], state.v[1].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[2].U16[6], state.v[2].U16[6], state.v[1].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[2].U16[7], state.v[2].U16[7], state.v[1].U16[7], 1, 0, 0, 1)
	}


_040019A8:	// 40019a8: <4b800010>	VADD      vec00 = vec00 + vec00[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[0].U16[3];
		VADD_operation(0, state.v[0].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019AC:	// 40019ac: <4a032100>	VMULF     vec04 = ( acc = (vec04 * vec03[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[4].U16[0], state.v[4].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[4].U16[1], state.v[4].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[4].U16[2], state.v[4].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[4].U16[3], state.v[4].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[4].U16[4], state.v[4].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[4].U16[5], state.v[4].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[4].U16[6], state.v[4].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[4].U16[7], state.v[4].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
	}


_040019B0:	// 40019b0: <4b929490>	VADD      vec18 = vec18 + vec18[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[18].U16[3];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_040019B4:	// 40019b4: <216bffff>	ADDI      T3 = T3 + ffff
	state.r[T3] = state.r[T3] + 0xFFFFFFFF;


_040019B8:	// 40019b8: <4a053180>	VMULF     vec06 = ( acc = (vec06 * vec05[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[6].U16[0], state.v[6].U16[0], state.v[5].U16[0], 1, 0, 0, 1)
		VMULF_operation(1, state.v[6].U16[1], state.v[6].U16[1], state.v[5].U16[1], 1, 0, 0, 1)
		VMULF_operation(2, state.v[6].U16[2], state.v[6].U16[2], state.v[5].U16[2], 1, 0, 0, 1)
		VMULF_operation(3, state.v[6].U16[3], state.v[6].U16[3], state.v[5].U16[3], 1, 0, 0, 1)
		VMULF_operation(4, state.v[6].U16[4], state.v[6].U16[4], state.v[5].U16[4], 1, 0, 0, 1)
		VMULF_operation(5, state.v[6].U16[5], state.v[6].U16[5], state.v[5].U16[5], 1, 0, 0, 1)
		VMULF_operation(6, state.v[6].U16[6], state.v[6].U16[6], state.v[5].U16[6], 1, 0, 0, 1)
		VMULF_operation(7, state.v[6].U16[7], state.v[6].U16[7], state.v[5].U16[7], 1, 0, 0, 1)
	}


_040019BC:	// 40019bc: <ea600801>	SSV       [S3 + 0x02] = vec00 <00>
	Save16_DMEM(state.v[0].U16[7], (0x00000002 + state.r[S3]) & 0xfff);


_040019C0:	// 40019c0: <4a074200>	VMULF     vec08 = ( acc = (vec08 * vec07[<none>]) << 1 ) >> 16
	{
		VMULF_operation(0, state.v[8].U16[0], state.v[8].U16[0], state.v[7].U16[0], 1, 1, 1, 1)
		VMULF_operation(1, state.v[8].U16[1], state.v[8].U16[1], state.v[7].U16[1], 1, 1, 1, 1)
		VMULF_operation(2, state.v[8].U16[2], state.v[8].U16[2], state.v[7].U16[2], 1, 1, 1, 1)
		VMULF_operation(3, state.v[8].U16[3], state.v[8].U16[3], state.v[7].U16[3], 1, 1, 1, 1)
		VMULF_operation(4, state.v[8].U16[4], state.v[8].U16[4], state.v[7].U16[4], 1, 1, 1, 1)
		VMULF_operation(5, state.v[8].U16[5], state.v[8].U16[5], state.v[7].U16[5], 1, 1, 1, 1)
		VMULF_operation(6, state.v[8].U16[6], state.v[8].U16[6], state.v[7].U16[6], 1, 1, 1, 1)
		VMULF_operation(7, state.v[8].U16[7], state.v[8].U16[7], state.v[7].U16[7], 1, 1, 1, 1)
	}


_040019C4:	// 40019c4: <ea720802>	SSV       [S3 + 0x04] = vec18 <00>
	Save16_DMEM(state.v[18].U16[7], (0x00000004 + state.r[S3]) & 0xfff);


_040019C8:	// 40019c8: <1d60ffda>	BGTZ      (R0>0) --> 1934
	sp_reg_pc = 0x040019C8;
	if((_s32)state.r[T3] > 0)
	{
		state.r[S3] = state.r[S3] + 0x0004;
		goto _04001934;
	}


_040019CC:	// 40019cc: <22730004>	ADDI      S3 = S3 + 0004
	state.r[S3] = state.r[S3] + 0x0004;


_040019D0:	// 40019d0: <22680002>	ADDI      T0 = S3 + 0002
	state.r[T0] = state.r[S3] + 0x0002;


_040019D4:	// 40019d4: <ca60207c>	LQV       vec00 <00> = [S3 + 0xc0]
	{
		_u32 addr = (0xFFFFFFC0 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[0].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[0].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[0].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[0].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[0].U64[0] = state.v[0].U64[0] & ~temp1;
			state.v[0].U64[1] = state.v[0].U64[1] & ~temp2;
			state.v[0].U64[0] = state.v[0].U64[0] | value1;
			state.v[0].U64[1] = state.v[0].U64[1] | value2;
		}
	}


_040019D8:	// 40019d8: <ca71207d>	LQV       vec17 <00> = [S3 + 0xd0]
	{
		_u32 addr = (0xFFFFFFD0 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_040019DC:	// 40019dc: <c902207e>	LQV       vec02 <00> = [T0 + 0xe0]
	{
		_u32 addr = (0xFFFFFFE0 + state.r[T0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_040019E0:	// 40019e0: <c902287f>	LRV       vec02 <00> = [T0 + 0xf0]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0xFFFFFFF0 + state.r[T0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[2].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040019E4:	// 40019e4: <c904207f>	LQV       vec04 <00> = [T0 + 0xf0]
	{
		_u32 addr = (0xFFFFFFF0 + state.r[T0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[4].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[4].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[4].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[4].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[4].U64[0] = state.v[4].U64[0] & ~temp1;
			state.v[4].U64[1] = state.v[4].U64[1] & ~temp2;
			state.v[4].U64[0] = state.v[4].U64[0] | value1;
			state.v[4].U64[1] = state.v[4].U64[1] | value2;
		}
	}


_040019E8:	// 40019e8: <4b340185>	VMUDM     vec06 = ( acc = vec00 * vec20[1] ) >> 16
	{
		_u16		s2value = state.v[20].U16[6];
		VMUDM_operation(0, state.v[6].U16[0], state.v[0].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[6].U16[1], state.v[0].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[6].U16[2], state.v[0].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[6].U16[3], state.v[0].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[6].U16[4], state.v[0].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[6].U16[5], state.v[0].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[6].U16[6], state.v[0].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[6].U16[7], state.v[0].U16[7], s2value, 0, 1, 0, 1)
	}


_040019EC:	// 40019ec: <c9042800>	LRV       vec04 <00> = [T0 + 0x00]
	{
		_s32		addr;
		_s32		offset;
		_u32		i, length;

		addr = (0x00000000 + state.r[T0]);
		offset = (addr & 0xf) - 1;
		length = (addr & 0xf);
		addr &= 0xff0;
		for (i=0; i<length; i++) {
			state.v[4].U8[offset - i] = pDMEM[(addr ^ 3)];
			addr++;
		}
	}


_040019F0:	// 40019f0: <4b14000f>	VMADH     vec00 = ( acc+= (vec00 * vec20[0]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMADH_operation(0, state.v[0].U16[0], state.v[0].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[0].U16[1], state.v[0].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[0].U16[2], state.v[0].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[0].U16[3], state.v[0].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[0].U16[4], state.v[0].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[0].U16[5], state.v[0].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[0].U16[6], state.v[0].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[0].U16[7], state.v[0].U16[7], s2value, 1, 0, 0, 1)
	}


_040019F4:	// 40019f4: <4b741185>	VMUDM     vec06 = ( acc = vec02 * vec20[3] ) >> 16
	{
		_u16		s2value = state.v[20].U16[4];
		VMUDM_operation(0, state.v[6].U16[0], state.v[2].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[6].U16[1], state.v[2].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[6].U16[2], state.v[2].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[6].U16[3], state.v[2].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[6].U16[4], state.v[2].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[6].U16[5], state.v[2].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[6].U16[6], state.v[2].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[6].U16[7], state.v[2].U16[7], s2value, 0, 1, 0, 1)
	}


_040019F8:	// 40019f8: <ea60207c>	SQV       [S3 + 0xc0] = vec00 <00>
	{
		_u32    addr = (0xFFFFFFC0 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[0].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[0].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[0].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[0].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[0].U64[0] & temp1);
			value2 |= (state.v[0].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_040019FC:	// 40019fc: <4b54108f>	VMADH     vec02 = ( acc+= (vec02 * vec20[2]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[5];
		VMADH_operation(0, state.v[2].U16[0], state.v[2].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[2].U16[1], state.v[2].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[2].U16[2], state.v[2].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[2].U16[3], state.v[2].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[2].U16[4], state.v[2].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[2].U16[5], state.v[2].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[2].U16[6], state.v[2].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[2].U16[7], state.v[2].U16[7], s2value, 1, 0, 0, 1)
	}


_04001A00:	// 4001a00: <4b348985>	VMUDM     vec06 = ( acc = vec17 * vec20[1] ) >> 16
	{
		_u16		s2value = state.v[20].U16[6];
		VMUDM_operation(0, state.v[6].U16[0], state.v[17].U16[0], s2value, 0, 1, 0, 1)
		VMUDM_operation(1, state.v[6].U16[1], state.v[17].U16[1], s2value, 0, 1, 0, 1)
		VMUDM_operation(2, state.v[6].U16[2], state.v[17].U16[2], s2value, 0, 1, 0, 1)
		VMUDM_operation(3, state.v[6].U16[3], state.v[17].U16[3], s2value, 0, 1, 0, 1)
		VMUDM_operation(4, state.v[6].U16[4], state.v[17].U16[4], s2value, 0, 1, 0, 1)
		VMUDM_operation(5, state.v[6].U16[5], state.v[17].U16[5], s2value, 0, 1, 0, 1)
		VMUDM_operation(6, state.v[6].U16[6], state.v[17].U16[6], s2value, 0, 1, 0, 1)
		VMUDM_operation(7, state.v[6].U16[7], state.v[17].U16[7], s2value, 0, 1, 0, 1)
	}


_04001A04:	// 4001a04: <e902207e>	SQV       [T0 + 0xe0] = vec02 <00>
	{
		_u32    addr = (0xFFFFFFE0 + state.r[T0]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[2].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[2].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[2].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[2].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[2].U64[0] & temp1);
			value2 |= (state.v[2].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A08:	// 4001a08: <e902287f>	SRV       [T0 + 0xf0] = vec02 <00>
	rsp_srv(0xE902287F);


_04001A0C:	// 4001a0c: <4b148c4f>	VMADH     vec17 = ( acc+= (vec17 * vec20[0]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[7];
		VMADH_operation(0, state.v[17].U16[0], state.v[17].U16[0], s2value, 1, 0, 0, 1)
		VMADH_operation(1, state.v[17].U16[1], state.v[17].U16[1], s2value, 1, 0, 0, 1)
		VMADH_operation(2, state.v[17].U16[2], state.v[17].U16[2], s2value, 1, 0, 0, 1)
		VMADH_operation(3, state.v[17].U16[3], state.v[17].U16[3], s2value, 1, 0, 0, 1)
		VMADH_operation(4, state.v[17].U16[4], state.v[17].U16[4], s2value, 1, 0, 0, 1)
		VMADH_operation(5, state.v[17].U16[5], state.v[17].U16[5], s2value, 1, 0, 0, 1)
		VMADH_operation(6, state.v[17].U16[6], state.v[17].U16[6], s2value, 1, 0, 0, 1)
		VMADH_operation(7, state.v[17].U16[7], state.v[17].U16[7], s2value, 1, 0, 0, 1)
	}


_04001A10:	// 4001a10: <4b742185>	VMUDM     vec06 = ( acc = vec04 * vec20[3] ) >> 16
	{
		_u16		s2value = state.v[20].U16[4];
		VMUDM_operation(0, state.v[6].U16[0], state.v[4].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[6].U16[1], state.v[4].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[6].U16[2], state.v[4].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[6].U16[3], state.v[4].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[6].U16[4], state.v[4].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[6].U16[5], state.v[4].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[6].U16[6], state.v[4].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[6].U16[7], state.v[4].U16[7], s2value, 1, 1, 1, 1)
	}


_04001A14:	// 4001a14: <ea71207d>	SQV       [S3 + 0xd0] = vec17 <00>
	{
		_u32    addr = (0xFFFFFFD0 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[17].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[17].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[17].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[17].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[17].U64[0] & temp1);
			value2 |= (state.v[17].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A18:	// 4001a18: <4b54210f>	VMADH     vec04 = ( acc+= (vec04 * vec20[2]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[5];
		VMADH_operation(0, state.v[4].U16[0], state.v[4].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[4].U16[1], state.v[4].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[4].U16[2], state.v[4].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[4].U16[3], state.v[4].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[4].U16[4], state.v[4].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[4].U16[5], state.v[4].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[4].U16[6], state.v[4].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[4].U16[7], state.v[4].U16[7], s2value, 1, 1, 0, 1)
	}


_04001A1C:	// 4001a1c: <e904207f>	SQV       [T0 + 0xf0] = vec04 <00>
	{
		_u32    addr = (0xFFFFFFF0 + state.r[T0]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[4].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[4].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[4].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[4].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[4].U64[0] & temp1);
			value2 |= (state.v[4].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001A20:	// 4001a20: <e9042800>	SRV       [T0 + 0x00] = vec04 <00>
	rsp_srv(0xE9042800);


_04001A24:	// 4001a24: <000e5820>	ADD       T3 = R0+T6
	state.r[T3] = (0 + state.r[T6]);


_04001A28:	// 4001a28: <000d7020>	ADD       T6 = R0+T5
	state.r[T6] = (0 + state.r[T5]);


_04001A2C:	// 4001a2c: <000b6820>	ADD       T5 = R0+T3
	state.r[T5] = (0 + state.r[T3]);


_04001A30:	// 4001a30: <218cfffe>	ADDI      T4 = T4 + fffe
	state.r[T4] = state.r[T4] + 0xFFFFFFFE;


_04001A34:	// 4001a34: <318c001e>	ANDI      T4 = T4 & 001e
	state.r[T4] = (state.r[T4] & 0x0000001E);


_04001A38:	// 4001a38: <2252ffc0>	ADDI      S2 = S2 + ffc0
	state.r[S2] = state.r[S2] + 0xFFFFFFC0;


_04001A3C:	// 4001a3c: <1e40fe24>	BGTZ      (R0>0) --> 12d0
	sp_reg_pc = 0x04001A3C;
	if((_s32)state.r[S2] > 0)
	{
		state.r[S7] = state.r[S7] + 0x0040;
		goto _040012D0;
	}


_04001A40:	// 4001a40: <22f70040>	ADDI      S7 = S7 + 0040
	state.r[S7] = state.r[S7] + 0x0040;


_04001A44:	// 4001a44: <20010e70>	ADDI      AT = R0 + 0e70
	state.r[AT] = 0 + 0x0E70;


_04001A48:	// 4001a48: <00161020>	ADD       V0 = R0+S6
	state.r[V0] = (0 + state.r[S6]);


_04001A4C:	// 4001a4c: <0d000466>	JAL	    04001198
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;
	sp_reg_pc = 0x04001A4C;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001A50:	// 4001a50: <2063ffff>	ADDI      V1 = V1 + ffff
	state.r[V1] = state.r[V1] + 0xFFFFFFFF;


_04001A54:	// 4001a54: <20630001>	ADDI      V1 = V1 + 0001
	state.r[V1] = state.r[V1] + 0x0001;


_04001A58:	// 4001a58: <22d60180>	ADDI      S6 = S6 + 0180
	state.r[S6] = state.r[S6] + 0x0180;


_04001A5C:	// 4001a5c: <22b50180>	ADDI      S5 = S5 + 0180
	state.r[S5] = state.r[S5] + 0x0180;


_04001A60:	// 4001a60: <0d00046d>	JAL	    040011b4
	state.r[V0] = (0 + state.r[S5]);
	sp_reg_pc = 0x04001A60;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001A64:	// 4001a64: <00151020>	ADD       V0 = R0+S5
	state.r[V0] = (0 + state.r[S5]);


_04001A68:	// 4001a68: <1e80fe11>	BGTZ      (R0>0) --> 12b0
	sp_reg_pc = 0x04001A68;
	if((_s32)state.r[S4] > 0)
	{
		state.r[AT] = 0 + 0x0CF0;
		goto _040012B0;
	}


_04001A6C:	// 4001a6c: <20010cf0>	ADDI      AT = R0 + 0cf0
	state.r[AT] = 0 + 0x0CF0;


_04001A70:	// 4001a70: <02201020>	ADD       V0 = S1+R0
	state.r[V0] = (state.r[S1] + 0);


_04001A74:	// 4001a74: <200102c0>	ADDI      AT = R0 + 02c0
	state.r[AT] = 0 + 0x02C0;


_04001A78:	// 4001a78: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x013F;
	sp_reg_pc = 0x04001A78;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001A7C:	// 4001a7c: <2003013f>	ADDI      V1 = R0 + 013f
	state.r[V1] = 0 + 0x013F;


_04001A80:	// 4001a80: <200108a0>	ADDI      AT = R0 + 08a0
	state.r[AT] = 0 + 0x08A0;


_04001A84:	// 4001a84: <0d000466>	JAL	    04001198
	state.r[V1] = 0 + 0x043F;
	sp_reg_pc = 0x04001A84;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001A88:	// 4001a88: <2003043f>	ADDI      V1 = R0 + 043f
	state.r[V1] = 0 + 0x043F;


_04001A8C:	// 4001a8c: <8c0b0ce8>	LW        T3 = [R0+0ce8]
	state.r[T3] = (_s32)Load32_DMEM((0 + 0x00000CE8) & 0xfff);


_04001A90:	// 4001a90: <0d00046d>	JAL	    040011b4
	sp_reg_pc = 0x04001A90;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _040011B4;


_04001A94:	// 4001a94: <00000000>	NOP       


_04001A98:	// 4001a98: <ac110ff4>	SW        [R0+0ff4] = S1
	Save32_DMEM((_u32)state.r[S1], (0 + 0x00000FF4) & 0xfff);


_04001A9C:	// 4001a9c: <ac100ff8>	SW        [R0+0ff8] = S0
	Save32_DMEM((_u32)state.r[S0], (0 + 0x00000FF8) & 0xfff);


_04001AA0:	// 4001aa0: <ac0f0ffc>	SW        [R0+0ffc] = T7
	Save32_DMEM((_u32)state.r[T7], (0 + 0x00000FFC) & 0xfff);


_04001AA4:	// 4001aa4: <ac0b0ff0>	SW        [R0+0ff0] = T3
	Save32_DMEM((_u32)state.r[T3], (0 + 0x00000FF0) & 0xfff);


_04001AA8:	// 4001aa8: <09000472>	J         040011c8
	goto _040011C8;


_04001AAC:	// 4001aac: <00000000>	NOP       


_04001AB0:	// 4001ab0: <4a080410>	VADD      vec16 = vec00 + vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[0].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[0].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[0].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[0].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[0].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[0].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[0].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AB4:	// 4001ab4: <4a090c50>	VADD      vec17 = vec01 + vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[1].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[1].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[1].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[1].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[1].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[1].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[1].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[1].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AB8:	// 4001ab8: <4a0a1490>	VADD      vec18 = vec02 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[18].U16[0], state.v[2].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[2].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[2].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[2].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[2].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[2].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[2].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[2].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001ABC:	// 4001abc: <4a0b1cd0>	VADD      vec19 = vec03 + vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[19].U16[0], state.v[3].U16[0], state.v[11].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[19].U16[1], state.v[3].U16[1], state.v[11].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[19].U16[2], state.v[3].U16[2], state.v[11].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[19].U16[3], state.v[3].U16[3], state.v[11].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[19].U16[4], state.v[3].U16[4], state.v[11].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[19].U16[5], state.v[3].U16[5], state.v[11].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[19].U16[6], state.v[3].U16[6], state.v[11].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[19].U16[7], state.v[3].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AC0:	// 4001ac0: <4a0c2510>	VADD      vec20 = vec04 + vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[20].U16[0], state.v[4].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[4].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[4].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[4].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[4].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[4].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[4].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[4].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AC4:	// 4001ac4: <4a0d2d50>	VADD      vec21 = vec05 + vec13[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[5].U16[0], state.v[13].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[5].U16[1], state.v[13].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[5].U16[2], state.v[13].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[5].U16[3], state.v[13].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[5].U16[4], state.v[13].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[5].U16[5], state.v[13].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[5].U16[6], state.v[13].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[5].U16[7], state.v[13].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AC8:	// 4001ac8: <4a0e3590>	VADD      vec22 = vec06 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[22].U16[0], state.v[6].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[22].U16[1], state.v[6].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[22].U16[2], state.v[6].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[22].U16[3], state.v[6].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[22].U16[4], state.v[6].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[22].U16[5], state.v[6].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[22].U16[6], state.v[6].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[22].U16[7], state.v[6].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001ACC:	// 4001acc: <4a0f3dd0>	VADD      vec23 = vec07 + vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[23].U16[0], state.v[7].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[23].U16[1], state.v[7].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[23].U16[2], state.v[7].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[23].U16[3], state.v[7].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[23].U16[4], state.v[7].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[23].U16[5], state.v[7].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[23].U16[6], state.v[7].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[23].U16[7], state.v[7].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AD0:	// 4001ad0: <4a080611>	VSUB      vec24 = vec00 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[24].U16[0], state.v[0].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[24].U16[1], state.v[0].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[24].U16[2], state.v[0].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[24].U16[3], state.v[0].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[24].U16[4], state.v[0].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[24].U16[5], state.v[0].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[24].U16[6], state.v[0].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[24].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AD4:	// 4001ad4: <c8082001>	LQV       vec08 <00> = [R0 + 0x10]
	{
		_u32 addr = (0x00000010 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[8].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[8].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[8].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[8].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[8].U64[0] = state.v[8].U64[0] & ~temp1;
			state.v[8].U64[1] = state.v[8].U64[1] & ~temp2;
			state.v[8].U64[0] = state.v[8].U64[0] | value1;
			state.v[8].U64[1] = state.v[8].U64[1] | value2;
		}
	}


_04001AD8:	// 4001ad8: <4a090e51>	VSUB      vec25 = vec01 - vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[25].U16[0], state.v[1].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[25].U16[1], state.v[1].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[25].U16[2], state.v[1].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[25].U16[3], state.v[1].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[25].U16[4], state.v[1].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[25].U16[5], state.v[1].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[25].U16[6], state.v[1].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[25].U16[7], state.v[1].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001ADC:	// 4001adc: <c8092002>	LQV       vec09 <00> = [R0 + 0x20]
	{
		_u32 addr = (0x00000020 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[9].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[9].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[9].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[9].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[9].U64[0] = state.v[9].U64[0] & ~temp1;
			state.v[9].U64[1] = state.v[9].U64[1] & ~temp2;
			state.v[9].U64[0] = state.v[9].U64[0] | value1;
			state.v[9].U64[1] = state.v[9].U64[1] | value2;
		}
	}


_04001AE0:	// 4001ae0: <4a0a1691>	VSUB      vec26 = vec02 - vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[26].U16[0], state.v[2].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[26].U16[1], state.v[2].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[26].U16[2], state.v[2].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[26].U16[3], state.v[2].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[26].U16[4], state.v[2].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[26].U16[5], state.v[2].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[26].U16[6], state.v[2].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[26].U16[7], state.v[2].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AE4:	// 4001ae4: <4a0b1ed1>	VSUB      vec27 = vec03 - vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[27].U16[0], state.v[3].U16[0], state.v[11].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[27].U16[1], state.v[3].U16[1], state.v[11].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[27].U16[2], state.v[3].U16[2], state.v[11].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[27].U16[3], state.v[3].U16[3], state.v[11].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[27].U16[4], state.v[3].U16[4], state.v[11].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[27].U16[5], state.v[3].U16[5], state.v[11].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[27].U16[6], state.v[3].U16[6], state.v[11].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[27].U16[7], state.v[3].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AE8:	// 4001ae8: <4b08c605>	VMUDM     vec24 = ( acc = vec24 * vec08[0] ) >> 16
	{
		_u16		s2value = state.v[8].U16[7];
		VMUDM_operation(0, state.v[24].U16[0], state.v[24].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[24].U16[1], state.v[24].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[24].U16[2], state.v[24].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[24].U16[3], state.v[24].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[24].U16[4], state.v[24].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[24].U16[5], state.v[24].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[24].U16[6], state.v[24].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[24].U16[7], state.v[24].U16[7], s2value, 1, 0, 0, 1)
	}


_04001AEC:	// 4001aec: <4b48ce45>	VMUDM     vec25 = ( acc = vec25 * vec08[2] ) >> 16
	{
		_u16		s2value = state.v[8].U16[5];
		VMUDM_operation(0, state.v[25].U16[0], state.v[25].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[25].U16[1], state.v[25].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[25].U16[2], state.v[25].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[25].U16[3], state.v[25].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[25].U16[4], state.v[25].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[25].U16[5], state.v[25].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[25].U16[6], state.v[25].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[25].U16[7], state.v[25].U16[7], s2value, 1, 0, 0, 1)
	}


_04001AF0:	// 4001af0: <4a0c2711>	VSUB      vec28 = vec04 - vec12[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[28].U16[0], state.v[4].U16[0], state.v[12].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[28].U16[1], state.v[4].U16[1], state.v[12].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[28].U16[2], state.v[4].U16[2], state.v[12].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[28].U16[3], state.v[4].U16[3], state.v[12].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[28].U16[4], state.v[4].U16[4], state.v[12].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[28].U16[5], state.v[4].U16[5], state.v[12].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[28].U16[6], state.v[4].U16[6], state.v[12].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[28].U16[7], state.v[4].U16[7], state.v[12].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AF4:	// 4001af4: <4a0d2f51>	VSUB      vec29 = vec05 - vec13[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[29].U16[0], state.v[5].U16[0], state.v[13].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[29].U16[1], state.v[5].U16[1], state.v[13].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[29].U16[2], state.v[5].U16[2], state.v[13].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[29].U16[3], state.v[5].U16[3], state.v[13].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[29].U16[4], state.v[5].U16[4], state.v[13].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[29].U16[5], state.v[5].U16[5], state.v[13].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[29].U16[6], state.v[5].U16[6], state.v[13].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[29].U16[7], state.v[5].U16[7], state.v[13].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001AF8:	// 4001af8: <4bc8d685>	VMUDM     vec26 = ( acc = vec26 * vec08[6] ) >> 16
	{
		_u16		s2value = state.v[8].U16[1];
		VMUDM_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 1, 0, 0, 1)
	}


_04001AFC:	// 4001afc: <4b88dec5>	VMUDM     vec27 = ( acc = vec27 * vec08[4] ) >> 16
	{
		_u16		s2value = state.v[8].U16[3];
		VMUDM_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B00:	// 4001b00: <4a0e3791>	VSUB      vec30 = vec06 - vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[30].U16[0], state.v[6].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[30].U16[1], state.v[6].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[30].U16[2], state.v[6].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[30].U16[3], state.v[6].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[30].U16[4], state.v[6].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[30].U16[5], state.v[6].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[30].U16[6], state.v[6].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[30].U16[7], state.v[6].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B04:	// 4001b04: <4a0f3fd1>	VSUB      vec31 = vec07 - vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[31].U16[0], state.v[7].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[31].U16[1], state.v[7].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[31].U16[2], state.v[7].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[31].U16[3], state.v[7].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[31].U16[4], state.v[7].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[31].U16[5], state.v[7].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[31].U16[6], state.v[7].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[31].U16[7], state.v[7].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B08:	// 4001b08: <4ba9e705>	VMUDM     vec28 = ( acc = vec28 * vec09[5] ) >> 16
	{
		_u16		s2value = state.v[9].U16[2];
		VMUDM_operation(0, state.v[28].U16[0], state.v[28].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[28].U16[1], state.v[28].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[28].U16[2], state.v[28].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[28].U16[3], state.v[28].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[28].U16[4], state.v[28].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[28].U16[5], state.v[28].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[28].U16[6], state.v[28].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[28].U16[7], state.v[28].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B0C:	// 4001b0c: <4b69ef45>	VMUDM     vec29 = ( acc = vec29 * vec09[3] ) >> 16
	{
		_u16		s2value = state.v[9].U16[4];
		VMUDM_operation(0, state.v[29].U16[0], state.v[29].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[29].U16[1], state.v[29].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[29].U16[2], state.v[29].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[29].U16[3], state.v[29].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[29].U16[4], state.v[29].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[29].U16[5], state.v[29].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[29].U16[6], state.v[29].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[29].U16[7], state.v[29].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B10:	// 4001b10: <4be8f785>	VMUDM     vec30 = ( acc = vec30 * vec08[7] ) >> 16
	{
		_u16		s2value = state.v[8].U16[0];
		VMUDM_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B14:	// 4001b14: <4b29ffc5>	VMUDM     vec31 = ( acc = vec31 * vec09[1] ) >> 16
	{
		_u16		s2value = state.v[9].U16[6];
		VMUDM_operation(0, state.v[31].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[31].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[31].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[31].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[31].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[31].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[31].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[31].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B18:	// 4001b18: <4a148111>	VSUB      vec04 = vec16 - vec20[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[4].U16[0], state.v[16].U16[0], state.v[20].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[4].U16[1], state.v[16].U16[1], state.v[20].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[4].U16[2], state.v[16].U16[2], state.v[20].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[4].U16[3], state.v[16].U16[3], state.v[20].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[4].U16[4], state.v[16].U16[4], state.v[20].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[4].U16[5], state.v[16].U16[5], state.v[20].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[4].U16[6], state.v[16].U16[6], state.v[20].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[4].U16[7], state.v[16].U16[7], state.v[20].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B1C:	// 4001b1c: <4a158951>	VSUB      vec05 = vec17 - vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[5].U16[0], state.v[17].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[5].U16[1], state.v[17].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[5].U16[2], state.v[17].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[5].U16[3], state.v[17].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[5].U16[4], state.v[17].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[5].U16[5], state.v[17].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[5].U16[6], state.v[17].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[5].U16[7], state.v[17].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B20:	// 4001b20: <4a148010>	VADD      vec00 = vec16 + vec20[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[0].U16[0], state.v[16].U16[0], state.v[20].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[0].U16[1], state.v[16].U16[1], state.v[20].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[0].U16[2], state.v[16].U16[2], state.v[20].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[0].U16[3], state.v[16].U16[3], state.v[20].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[0].U16[4], state.v[16].U16[4], state.v[20].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[0].U16[5], state.v[16].U16[5], state.v[20].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[0].U16[6], state.v[16].U16[6], state.v[20].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[0].U16[7], state.v[16].U16[7], state.v[20].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B24:	// 4001b24: <4a158850>	VADD      vec01 = vec17 + vec21[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[1].U16[0], state.v[17].U16[0], state.v[21].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[1].U16[1], state.v[17].U16[1], state.v[21].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[1].U16[2], state.v[17].U16[2], state.v[21].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[1].U16[3], state.v[17].U16[3], state.v[21].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[1].U16[4], state.v[17].U16[4], state.v[21].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[1].U16[5], state.v[17].U16[5], state.v[21].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[1].U16[6], state.v[17].U16[6], state.v[21].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[1].U16[7], state.v[17].U16[7], state.v[21].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B28:	// 4001b28: <4b282105>	VMUDM     vec04 = ( acc = vec04 * vec08[1] ) >> 16
	{
		_u16		s2value = state.v[8].U16[6];
		VMUDM_operation(0, state.v[4].U16[0], state.v[4].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[4].U16[1], state.v[4].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[4].U16[2], state.v[4].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[4].U16[3], state.v[4].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[4].U16[4], state.v[4].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[4].U16[5], state.v[4].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[4].U16[6], state.v[4].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[4].U16[7], state.v[4].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B2C:	// 4001b2c: <4ba82945>	VMUDM     vec05 = ( acc = vec05 * vec08[5] ) >> 16
	{
		_u16		s2value = state.v[8].U16[2];
		VMUDM_operation(0, state.v[5].U16[0], state.v[5].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[5].U16[1], state.v[5].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[5].U16[2], state.v[5].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[5].U16[3], state.v[5].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[5].U16[4], state.v[5].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[5].U16[5], state.v[5].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[5].U16[6], state.v[5].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[5].U16[7], state.v[5].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B30:	// 4001b30: <4a169191>	VSUB      vec06 = vec18 - vec22[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[6].U16[0], state.v[18].U16[0], state.v[22].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[6].U16[1], state.v[18].U16[1], state.v[22].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[6].U16[2], state.v[18].U16[2], state.v[22].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[6].U16[3], state.v[18].U16[3], state.v[22].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[6].U16[4], state.v[18].U16[4], state.v[22].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[6].U16[5], state.v[18].U16[5], state.v[22].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[6].U16[6], state.v[18].U16[6], state.v[22].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[6].U16[7], state.v[18].U16[7], state.v[22].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B34:	// 4001b34: <4a1799d1>	VSUB      vec07 = vec19 - vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[7].U16[0], state.v[19].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[7].U16[1], state.v[19].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[7].U16[2], state.v[19].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[7].U16[3], state.v[19].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[7].U16[4], state.v[19].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[7].U16[5], state.v[19].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[7].U16[6], state.v[19].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[7].U16[7], state.v[19].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B38:	// 4001b38: <4a169090>	VADD      vec02 = vec18 + vec22[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[2].U16[0], state.v[18].U16[0], state.v[22].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[2].U16[1], state.v[18].U16[1], state.v[22].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[2].U16[2], state.v[18].U16[2], state.v[22].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[2].U16[3], state.v[18].U16[3], state.v[22].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[2].U16[4], state.v[18].U16[4], state.v[22].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[2].U16[5], state.v[18].U16[5], state.v[22].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[2].U16[6], state.v[18].U16[6], state.v[22].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[2].U16[7], state.v[18].U16[7], state.v[22].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B3C:	// 4001b3c: <4a1798d0>	VADD      vec03 = vec19 + vec23[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[3].U16[0], state.v[19].U16[0], state.v[23].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[3].U16[1], state.v[19].U16[1], state.v[23].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[3].U16[2], state.v[19].U16[2], state.v[23].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[3].U16[3], state.v[19].U16[3], state.v[23].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[3].U16[4], state.v[19].U16[4], state.v[23].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[3].U16[5], state.v[19].U16[5], state.v[23].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[3].U16[6], state.v[19].U16[6], state.v[23].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[3].U16[7], state.v[19].U16[7], state.v[23].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B40:	// 4001b40: <4b893185>	VMUDM     vec06 = ( acc = vec06 * vec09[4] ) >> 16
	{
		_u16		s2value = state.v[9].U16[3];
		VMUDM_operation(0, state.v[6].U16[0], state.v[6].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[6].U16[1], state.v[6].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[6].U16[2], state.v[6].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[6].U16[3], state.v[6].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[6].U16[4], state.v[6].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[6].U16[5], state.v[6].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[6].U16[6], state.v[6].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[6].U16[7], state.v[6].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B44:	// 4001b44: <4b0939c5>	VMUDM     vec07 = ( acc = vec07 * vec09[0] ) >> 16
	{
		_u16		s2value = state.v[9].U16[7];
		VMUDM_operation(0, state.v[7].U16[0], state.v[7].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[7].U16[1], state.v[7].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[7].U16[2], state.v[7].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[7].U16[3], state.v[7].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[7].U16[4], state.v[7].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[7].U16[5], state.v[7].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[7].U16[6], state.v[7].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[7].U16[7], state.v[7].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B48:	// 4001b48: <4a1cc311>	VSUB      vec12 = vec24 - vec28[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[12].U16[0], state.v[24].U16[0], state.v[28].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[12].U16[1], state.v[24].U16[1], state.v[28].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[12].U16[2], state.v[24].U16[2], state.v[28].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[12].U16[3], state.v[24].U16[3], state.v[28].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[12].U16[4], state.v[24].U16[4], state.v[28].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[12].U16[5], state.v[24].U16[5], state.v[28].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[12].U16[6], state.v[24].U16[6], state.v[28].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[12].U16[7], state.v[24].U16[7], state.v[28].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B4C:	// 4001b4c: <4a1dcb51>	VSUB      vec13 = vec25 - vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[13].U16[0], state.v[25].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[13].U16[1], state.v[25].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[13].U16[2], state.v[25].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[13].U16[3], state.v[25].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[13].U16[4], state.v[25].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[13].U16[5], state.v[25].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[13].U16[6], state.v[25].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[13].U16[7], state.v[25].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B50:	// 4001b50: <4a1ed290>	VADD      vec10 = vec26 + vec30[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[10].U16[0], state.v[26].U16[0], state.v[30].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[10].U16[1], state.v[26].U16[1], state.v[30].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[10].U16[2], state.v[26].U16[2], state.v[30].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[10].U16[3], state.v[26].U16[3], state.v[30].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[10].U16[4], state.v[26].U16[4], state.v[30].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[10].U16[5], state.v[26].U16[5], state.v[30].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[10].U16[6], state.v[26].U16[6], state.v[30].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[10].U16[7], state.v[26].U16[7], state.v[30].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B54:	// 4001b54: <4a1fdad0>	VADD      vec11 = vec27 + vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[11].U16[0], state.v[27].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[11].U16[1], state.v[27].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[11].U16[2], state.v[27].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[11].U16[3], state.v[27].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[11].U16[4], state.v[27].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[11].U16[5], state.v[27].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[11].U16[6], state.v[27].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[11].U16[7], state.v[27].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B58:	// 4001b58: <4a1ed391>	VSUB      vec14 = vec26 - vec30[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[14].U16[0], state.v[26].U16[0], state.v[30].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[14].U16[1], state.v[26].U16[1], state.v[30].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[14].U16[2], state.v[26].U16[2], state.v[30].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[14].U16[3], state.v[26].U16[3], state.v[30].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[14].U16[4], state.v[26].U16[4], state.v[30].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[14].U16[5], state.v[26].U16[5], state.v[30].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[14].U16[6], state.v[26].U16[6], state.v[30].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[14].U16[7], state.v[26].U16[7], state.v[30].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B5C:	// 4001b5c: <4a1fdbd1>	VSUB      vec15 = vec27 - vec31[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[15].U16[0], state.v[27].U16[0], state.v[31].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[15].U16[1], state.v[27].U16[1], state.v[31].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[15].U16[2], state.v[27].U16[2], state.v[31].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[15].U16[3], state.v[27].U16[3], state.v[31].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[15].U16[4], state.v[27].U16[4], state.v[31].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[15].U16[5], state.v[27].U16[5], state.v[31].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[15].U16[6], state.v[27].U16[6], state.v[31].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[15].U16[7], state.v[27].U16[7], state.v[31].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B60:	// 4001b60: <4b286305>	VMUDM     vec12 = ( acc = vec12 * vec08[1] ) >> 16
	{
		_u16		s2value = state.v[8].U16[6];
		VMUDM_operation(0, state.v[12].U16[0], state.v[12].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[12].U16[1], state.v[12].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[12].U16[2], state.v[12].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[12].U16[3], state.v[12].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[12].U16[4], state.v[12].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[12].U16[5], state.v[12].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[12].U16[6], state.v[12].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[12].U16[7], state.v[12].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B64:	// 4001b64: <4ba86b45>	VMUDM     vec13 = ( acc = vec13 * vec08[5] ) >> 16
	{
		_u16		s2value = state.v[8].U16[2];
		VMUDM_operation(0, state.v[13].U16[0], state.v[13].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[13].U16[1], state.v[13].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[13].U16[2], state.v[13].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[13].U16[3], state.v[13].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[13].U16[4], state.v[13].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[13].U16[5], state.v[13].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[13].U16[6], state.v[13].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[13].U16[7], state.v[13].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B68:	// 4001b68: <4b897385>	VMUDM     vec14 = ( acc = vec14 * vec09[4] ) >> 16
	{
		_u16		s2value = state.v[9].U16[3];
		VMUDM_operation(0, state.v[14].U16[0], state.v[14].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[14].U16[1], state.v[14].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[14].U16[2], state.v[14].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[14].U16[3], state.v[14].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[14].U16[4], state.v[14].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[14].U16[5], state.v[14].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[14].U16[6], state.v[14].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[14].U16[7], state.v[14].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B6C:	// 4001b6c: <4b097bc5>	VMUDM     vec15 = ( acc = vec15 * vec09[0] ) >> 16
	{
		_u16		s2value = state.v[9].U16[7];
		VMUDM_operation(0, state.v[15].U16[0], state.v[15].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[15].U16[1], state.v[15].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[15].U16[2], state.v[15].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[15].U16[3], state.v[15].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[15].U16[4], state.v[15].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[15].U16[5], state.v[15].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[15].U16[6], state.v[15].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[15].U16[7], state.v[15].U16[7], s2value, 1, 0, 0, 1)
	}


_04001B70:	// 4001b70: <4a1cc210>	VADD      vec08 = vec24 + vec28[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[8].U16[0], state.v[24].U16[0], state.v[28].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[8].U16[1], state.v[24].U16[1], state.v[28].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[8].U16[2], state.v[24].U16[2], state.v[28].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[8].U16[3], state.v[24].U16[3], state.v[28].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[8].U16[4], state.v[24].U16[4], state.v[28].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[8].U16[5], state.v[24].U16[5], state.v[28].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[8].U16[6], state.v[24].U16[6], state.v[28].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[8].U16[7], state.v[24].U16[7], state.v[28].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B74:	// 4001b74: <4a1dca50>	VADD      vec09 = vec25 + vec29[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[9].U16[0], state.v[25].U16[0], state.v[29].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[9].U16[1], state.v[25].U16[1], state.v[29].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[9].U16[2], state.v[25].U16[2], state.v[29].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[9].U16[3], state.v[25].U16[3], state.v[29].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[9].U16[4], state.v[25].U16[4], state.v[29].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[9].U16[5], state.v[25].U16[5], state.v[29].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[9].U16[6], state.v[25].U16[6], state.v[29].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[9].U16[7], state.v[25].U16[7], state.v[29].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B78:	// 4001b78: <4a020410>	VADD      vec16 = vec00 + vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[16].U16[0], state.v[0].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[16].U16[1], state.v[0].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[16].U16[2], state.v[0].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[16].U16[3], state.v[0].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[16].U16[4], state.v[0].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[16].U16[5], state.v[0].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[16].U16[6], state.v[0].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[16].U16[7], state.v[0].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B7C:	// 4001b7c: <4a030c50>	VADD      vec17 = vec01 + vec03[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[17].U16[0], state.v[1].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[17].U16[1], state.v[1].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[17].U16[2], state.v[1].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[17].U16[3], state.v[1].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[17].U16[4], state.v[1].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[17].U16[5], state.v[1].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[17].U16[6], state.v[1].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[17].U16[7], state.v[1].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B80:	// 4001b80: <4a020491>	VSUB      vec18 = vec00 - vec02[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[18].U16[0], state.v[0].U16[0], state.v[2].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[18].U16[1], state.v[0].U16[1], state.v[2].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[18].U16[2], state.v[0].U16[2], state.v[2].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[18].U16[3], state.v[0].U16[3], state.v[2].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[18].U16[4], state.v[0].U16[4], state.v[2].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[18].U16[5], state.v[0].U16[5], state.v[2].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[18].U16[6], state.v[0].U16[6], state.v[2].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[18].U16[7], state.v[0].U16[7], state.v[2].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B84:	// 4001b84: <c8002001>	LQV       vec00 <00> = [R0 + 0x10]
	{
		_u32 addr = (0x00000010 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[0].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[0].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[0].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[0].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[0].U64[0] = state.v[0].U64[0] & ~temp1;
			state.v[0].U64[1] = state.v[0].U64[1] & ~temp2;
			state.v[0].U64[0] = state.v[0].U64[0] | value1;
			state.v[0].U64[1] = state.v[0].U64[1] | value2;
		}
	}


_04001B88:	// 4001b88: <4a030cd1>	VSUB      vec19 = vec01 - vec03[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[19].U16[0], state.v[1].U16[0], state.v[3].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[19].U16[1], state.v[1].U16[1], state.v[3].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[19].U16[2], state.v[1].U16[2], state.v[3].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[19].U16[3], state.v[1].U16[3], state.v[3].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[19].U16[4], state.v[1].U16[4], state.v[3].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[19].U16[5], state.v[1].U16[5], state.v[3].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[19].U16[6], state.v[1].U16[6], state.v[3].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[19].U16[7], state.v[1].U16[7], state.v[3].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B8C:	// 4001b8c: <c8022002>	LQV       vec02 <00> = [R0 + 0x20]
	{
		_u32 addr = (0x00000020 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[2].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[2].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[2].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[2].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[2].U64[0] = state.v[2].U64[0] & ~temp1;
			state.v[2].U64[1] = state.v[2].U64[1] & ~temp2;
			state.v[2].U64[0] = state.v[2].U64[0] | value1;
			state.v[2].U64[1] = state.v[2].U64[1] | value2;
		}
	}


_04001B90:	// 4001b90: <4a062510>	VADD      vec20 = vec04 + vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[20].U16[0], state.v[4].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[4].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[4].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[4].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[4].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[4].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[4].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[4].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B94:	// 4001b94: <4a072d50>	VADD      vec21 = vec05 + vec07[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[21].U16[0], state.v[5].U16[0], state.v[7].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[21].U16[1], state.v[5].U16[1], state.v[7].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[21].U16[2], state.v[5].U16[2], state.v[7].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[21].U16[3], state.v[5].U16[3], state.v[7].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[21].U16[4], state.v[5].U16[4], state.v[7].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[21].U16[5], state.v[5].U16[5], state.v[7].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[21].U16[6], state.v[5].U16[6], state.v[7].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[21].U16[7], state.v[5].U16[7], state.v[7].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B98:	// 4001b98: <4a062591>	VSUB      vec22 = vec04 - vec06[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[22].U16[0], state.v[4].U16[0], state.v[6].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[22].U16[1], state.v[4].U16[1], state.v[6].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[22].U16[2], state.v[4].U16[2], state.v[6].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[22].U16[3], state.v[4].U16[3], state.v[6].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[22].U16[4], state.v[4].U16[4], state.v[6].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[22].U16[5], state.v[4].U16[5], state.v[6].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[22].U16[6], state.v[4].U16[6], state.v[6].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[22].U16[7], state.v[4].U16[7], state.v[6].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001B9C:	// 4001b9c: <4a072dd1>	VSUB      vec23 = vec05 - vec07[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[23].U16[0], state.v[5].U16[0], state.v[7].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[23].U16[1], state.v[5].U16[1], state.v[7].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[23].U16[2], state.v[5].U16[2], state.v[7].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[23].U16[3], state.v[5].U16[3], state.v[7].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[23].U16[4], state.v[5].U16[4], state.v[7].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[23].U16[5], state.v[5].U16[5], state.v[7].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[23].U16[6], state.v[5].U16[6], state.v[7].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[23].U16[7], state.v[5].U16[7], state.v[7].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BA0:	// 4001ba0: <4b609485>	VMUDM     vec18 = ( acc = vec18 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BA4:	// 4001ba4: <4b429cc5>	VMUDM     vec19 = ( acc = vec19 * vec02[2] ) >> 16
	{
		_u16		s2value = state.v[2].U16[5];
		VMUDM_operation(0, state.v[19].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[19].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[19].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[19].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[19].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[19].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[19].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[19].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BA8:	// 4001ba8: <4b60b585>	VMUDM     vec22 = ( acc = vec22 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[22].U16[0], state.v[22].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[22].U16[1], state.v[22].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[22].U16[2], state.v[22].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[22].U16[3], state.v[22].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[22].U16[4], state.v[22].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[22].U16[5], state.v[22].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[22].U16[6], state.v[22].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[22].U16[7], state.v[22].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BAC:	// 4001bac: <4b42bdc5>	VMUDM     vec23 = ( acc = vec23 * vec02[2] ) >> 16
	{
		_u16		s2value = state.v[2].U16[5];
		VMUDM_operation(0, state.v[23].U16[0], state.v[23].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[23].U16[1], state.v[23].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[23].U16[2], state.v[23].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[23].U16[3], state.v[23].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[23].U16[4], state.v[23].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[23].U16[5], state.v[23].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[23].U16[6], state.v[23].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[23].U16[7], state.v[23].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BB0:	// 4001bb0: <4a0a4691>	VSUB      vec26 = vec08 - vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[26].U16[0], state.v[8].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[26].U16[1], state.v[8].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[26].U16[2], state.v[8].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[26].U16[3], state.v[8].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[26].U16[4], state.v[8].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[26].U16[5], state.v[8].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[26].U16[6], state.v[8].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[26].U16[7], state.v[8].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BB4:	// 4001bb4: <4a0b4ed1>	VSUB      vec27 = vec09 - vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[27].U16[0], state.v[9].U16[0], state.v[11].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[27].U16[1], state.v[9].U16[1], state.v[11].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[27].U16[2], state.v[9].U16[2], state.v[11].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[27].U16[3], state.v[9].U16[3], state.v[11].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[27].U16[4], state.v[9].U16[4], state.v[11].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[27].U16[5], state.v[9].U16[5], state.v[11].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[27].U16[6], state.v[9].U16[6], state.v[11].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[27].U16[7], state.v[9].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BB8:	// 4001bb8: <4a0a4610>	VADD      vec24 = vec08 + vec10[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[24].U16[0], state.v[8].U16[0], state.v[10].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[24].U16[1], state.v[8].U16[1], state.v[10].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[24].U16[2], state.v[8].U16[2], state.v[10].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[24].U16[3], state.v[8].U16[3], state.v[10].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[24].U16[4], state.v[8].U16[4], state.v[10].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[24].U16[5], state.v[8].U16[5], state.v[10].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[24].U16[6], state.v[8].U16[6], state.v[10].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[24].U16[7], state.v[8].U16[7], state.v[10].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BBC:	// 4001bbc: <4a0b4e50>	VADD      vec25 = vec09 + vec11[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[25].U16[0], state.v[9].U16[0], state.v[11].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[25].U16[1], state.v[9].U16[1], state.v[11].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[25].U16[2], state.v[9].U16[2], state.v[11].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[25].U16[3], state.v[9].U16[3], state.v[11].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[25].U16[4], state.v[9].U16[4], state.v[11].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[25].U16[5], state.v[9].U16[5], state.v[11].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[25].U16[6], state.v[9].U16[6], state.v[11].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[25].U16[7], state.v[9].U16[7], state.v[11].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BC0:	// 4001bc0: <4b60d685>	VMUDM     vec26 = ( acc = vec26 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BC4:	// 4001bc4: <4b42dec5>	VMUDM     vec27 = ( acc = vec27 * vec02[2] ) >> 16
	{
		_u16		s2value = state.v[2].U16[5];
		VMUDM_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BC8:	// 4001bc8: <4a0e6791>	VSUB      vec30 = vec12 - vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[30].U16[0], state.v[12].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[30].U16[1], state.v[12].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[30].U16[2], state.v[12].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[30].U16[3], state.v[12].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[30].U16[4], state.v[12].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[30].U16[5], state.v[12].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[30].U16[6], state.v[12].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[30].U16[7], state.v[12].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BCC:	// 4001bcc: <4a0f6fd1>	VSUB      vec31 = vec13 - vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[31].U16[0], state.v[13].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[31].U16[1], state.v[13].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[31].U16[2], state.v[13].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[31].U16[3], state.v[13].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[31].U16[4], state.v[13].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[31].U16[5], state.v[13].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[31].U16[6], state.v[13].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[31].U16[7], state.v[13].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BD0:	// 4001bd0: <4a0e6710>	VADD      vec28 = vec12 + vec14[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[28].U16[0], state.v[12].U16[0], state.v[14].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[28].U16[1], state.v[12].U16[1], state.v[14].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[28].U16[2], state.v[12].U16[2], state.v[14].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[28].U16[3], state.v[12].U16[3], state.v[14].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[28].U16[4], state.v[12].U16[4], state.v[14].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[28].U16[5], state.v[12].U16[5], state.v[14].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[28].U16[6], state.v[12].U16[6], state.v[14].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[28].U16[7], state.v[12].U16[7], state.v[14].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BD4:	// 4001bd4: <4a0f6f50>	VADD      vec29 = vec13 + vec15[<none>]
	{
		_u16		flag0 = state.flag[0];
		VADD_operation(0, state.v[29].U16[0], state.v[13].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VADD_operation(1, state.v[29].U16[1], state.v[13].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VADD_operation(2, state.v[29].U16[2], state.v[13].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VADD_operation(3, state.v[29].U16[3], state.v[13].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VADD_operation(4, state.v[29].U16[4], state.v[13].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VADD_operation(5, state.v[29].U16[5], state.v[13].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VADD_operation(6, state.v[29].U16[6], state.v[13].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VADD_operation(7, state.v[29].U16[7], state.v[13].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001BD8:	// 4001bd8: <4b60f785>	VMUDM     vec30 = ( acc = vec30 * vec00[3] ) >> 16
	{
		_u16		s2value = state.v[0].U16[4];
		VMUDM_operation(0, state.v[30].U16[0], state.v[30].U16[0], s2value, 1, 0, 0, 1)
		VMUDM_operation(1, state.v[30].U16[1], state.v[30].U16[1], s2value, 1, 0, 0, 1)
		VMUDM_operation(2, state.v[30].U16[2], state.v[30].U16[2], s2value, 1, 0, 0, 1)
		VMUDM_operation(3, state.v[30].U16[3], state.v[30].U16[3], s2value, 1, 0, 0, 1)
		VMUDM_operation(4, state.v[30].U16[4], state.v[30].U16[4], s2value, 1, 0, 0, 1)
		VMUDM_operation(5, state.v[30].U16[5], state.v[30].U16[5], s2value, 1, 0, 0, 1)
		VMUDM_operation(6, state.v[30].U16[6], state.v[30].U16[6], s2value, 1, 0, 0, 1)
		VMUDM_operation(7, state.v[30].U16[7], state.v[30].U16[7], s2value, 1, 0, 0, 1)
	}


_04001BDC:	// 4001bdc: <4b42ffc5>	VMUDM     vec31 = ( acc = vec31 * vec02[2] ) >> 16
	{
		_u16		s2value = state.v[2].U16[5];
		VMUDM_operation(0, state.v[31].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMUDM_operation(1, state.v[31].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMUDM_operation(2, state.v[31].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMUDM_operation(3, state.v[31].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMUDM_operation(4, state.v[31].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMUDM_operation(5, state.v[31].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMUDM_operation(6, state.v[31].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMUDM_operation(7, state.v[31].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


_04001BE0:	// 4001be0: <03e00008>	JR        RA
	{
		_u32	temp = ((state.r[RA] & 0x0FFF) >> 2);
		_u32	tempAddr = JumpTable[temp];
		__asm mov eax, tempAddr;
		__asm jmp eax;
	}


_04001BE4:	// 4001be4: <00000000>	NOP       


_04001BE8:	// 4001be8: <00000000>	NOP       


_04001BEC:	// 4001bec: <00000000>	NOP       


_04001BF0:	// 4001bf0: <00000000>	NOP       


_04001BF4:	// 4001bf4: <00000000>	NOP       


_04001BF8:	// 4001bf8: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	{
		_u16		s2value = state.v[21].U16[2];
		VMUDL_operation(0, state.v[20].U16[0], state.v[24].U16[0], s2value, 1, 1, 1, 1)
		VMUDL_operation(1, state.v[20].U16[1], state.v[24].U16[1], s2value, 1, 1, 1, 1)
		VMUDL_operation(2, state.v[20].U16[2], state.v[24].U16[2], s2value, 1, 1, 1, 1)
		VMUDL_operation(3, state.v[20].U16[3], state.v[24].U16[3], s2value, 1, 1, 1, 1)
		VMUDL_operation(4, state.v[20].U16[4], state.v[24].U16[4], s2value, 1, 1, 1, 1)
		VMUDL_operation(5, state.v[20].U16[5], state.v[24].U16[5], s2value, 1, 1, 1, 1)
		VMUDL_operation(6, state.v[20].U16[6], state.v[24].U16[6], s2value, 1, 1, 1, 1)
		VMUDL_operation(7, state.v[20].U16[7], state.v[24].U16[7], s2value, 1, 1, 1, 1)
	}


_04001BFC:	// 4001bfc: <10800002>	BEQ       (R0==A0) --> 1c08
	sp_reg_pc = 0x04001BFC;
	if (state.r[A0] == 0)
	{
		state.r[S7] = 0 + 0x0500;
		goto _04001C08;
	}


_04001C00:	// 4001c00: <20170500>	ADDI      S7 = R0 + 0500
	state.r[S7] = 0 + 0x0500;


_04001C04:	// 4001c04: <20170670>	ADDI      S7 = R0 + 0670
	state.r[S7] = 0 + 0x0670;


_04001C08:	// 4001c08: <eb180b84>	SSV       [T8 + 0x08] = vec24 <07>
	Save16_DMEM(state.v[24].U16[4], (0x00000008 + state.r[T8]) & 0xfff);


_04001C0C:	// 4001c0c: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	{
		_u16		s2value = state.v[30].U16[3];
		VMUDN_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 0, 1, 1, 1)
		VMUDN_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 0, 1, 1, 1)
		VMUDN_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 0, 1, 1, 1)
		VMUDN_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 0, 1, 1, 1)
		VMUDN_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 0, 1, 1, 1)
		VMUDN_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 0, 1, 1, 1)
		VMUDN_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 0, 1, 1, 1)
		VMUDN_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 0, 1, 1, 1)
	}


_04001C10:	// 4001c10: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	{
		_u32    addr = (0xFFFFFFB0 + 0);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[22].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[22].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[22].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[22].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[22].U64[0] & temp1);
			value2 |= (state.v[22].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C14:	// 4001c14: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	{
		_u16		s2value = state.v[21].U16[4];
		VMADN_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001C18:	// 4001c18: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	{
		_u32    addr = (0xFFFFFFC0 + 0);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[20].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[20].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[20].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[20].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[20].U64[0] & temp1);
			value2 |= (state.v[20].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001C1C:	// 4001c1c: <84150fb0>	LH        S5 = [R0+0fb0]
	state.r[S5] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB0) & 0xfff);


_04001C20:	// 4001c20: <840d0fc0>	LH        T5 = [R0+0fc0]
	state.r[T5] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC0) & 0xfff);


_04001C24:	// 4001c24: <84110fb8>	LH        S1 = [R0+0fb8]
	state.r[S1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB8) & 0xfff);


_04001C28:	// 4001c28: <84090fc8>	LH        T1 = [R0+0fc8]
	state.r[T1] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC8) & 0xfff);


_04001C2C:	// 4001c2c: <84140fb2>	LH        S4 = [R0+0fb2]
	state.r[S4] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB2) & 0xfff);


_04001C30:	// 4001c30: <840c0fc2>	LH        T4 = [R0+0fc2]
	state.r[T4] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC2) & 0xfff);


_04001C34:	// 4001c34: <84100fba>	LH        S0 = [R0+0fba]
	state.r[S0] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBA) & 0xfff);


_04001C38:	// 4001c38: <84080fca>	LH        T0 = [R0+0fca]
	state.r[T0] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCA) & 0xfff);


_04001C3C:	// 4001c3c: <84130fb4>	LH        S3 = [R0+0fb4]
	state.r[S3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB4) & 0xfff);


_04001C40:	// 4001c40: <840b0fc4>	LH        T3 = [R0+0fc4]
	state.r[T3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC4) & 0xfff);


_04001C44:	// 4001c44: <840f0fbc>	LH        T7 = [R0+0fbc]
	state.r[T7] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBC) & 0xfff);


_04001C48:	// 4001c48: <84070fcc>	LH        A3 = [R0+0fcc]
	state.r[A3] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCC) & 0xfff);


_04001C4C:	// 4001c4c: <84120fb6>	LH        S2 = [R0+0fb6]
	state.r[S2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FB6) & 0xfff);


_04001C50:	// 4001c50: <840a0fc6>	LH        T2 = [R0+0fc6]
	state.r[T2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FC6) & 0xfff);


_04001C54:	// 4001c54: <840e0fbe>	LH        T6 = [R0+0fbe]
	state.r[T6] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FBE) & 0xfff);


_04001C58:	// 4001c58: <84060fce>	LH        A2 = [R0+0fce]
	state.r[A2] = (_s32)(_s16)Load16_DMEM((0 + 0x00000FCE) & 0xfff);


_04001C5C:	// 4001c5c: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	// In function: SmashBros193C_1A28
	//Call the function: SmashBros193C_1A28
	SmashBros193C_1A28();
	goto _04001D4C;

_04001C60:	// 4001c60: <4bf8fe05>	VMUDM     vec24 = ( acc = vec31 * vec24[7] ) >> 16
	ErrorToCallFunction(0x04001C60);

_04001C64:	// 4001c64: <c9b21800>	LDV       vec18 <00> = [T5 + 0x00]
	ErrorToCallFunction(0x04001C64);

_04001C68:	// 4001c68: <4bf7fe0f>	VMADH     vec24 = ( acc+= (vec31 * vec23[7]) << 16) >> 16
	ErrorToCallFunction(0x04001C68);

_04001C6C:	// 4001c6c: <ca331c00>	LDV       vec19 <08> = [S1 + 0x00]
	ErrorToCallFunction(0x04001C6C);

_04001C70:	// 4001c70: <4b95cdcd>	VMADM     vec23 = ( acc+= vec25 * vec21[4] ) >> 16
	ErrorToCallFunction(0x04001C70);

_04001C74:	// 4001c74: <c9321c00>	LDV       vec18 <08> = [T1 + 0x00]
	ErrorToCallFunction(0x04001C74);

_04001C78:	// 4001c78: <4b1efe0e>	VMADN     vec24 = ( acc+= vec31 * vec30[0] )
	ErrorToCallFunction(0x04001C78);

_04001C7C:	// 4001c7c: <ca911800>	LDV       vec17 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001C7C);

_04001C80:	// 4001c80: <4b55fd86>	VMUDN     vec22 = ( acc = vec31 * vec21[2]      ) >> 16
	ErrorToCallFunction(0x04001C80);

_04001C84:	// 4001c84: <c9901800>	LDV       vec16 <00> = [T4 + 0x00]
	ErrorToCallFunction(0x04001C84);

_04001C88:	// 4001c88: <ca111c00>	LDV       vec17 <08> = [S0 + 0x00]
	ErrorToCallFunction(0x04001C88);

_04001C8C:	// 4001c8c: <4b5ebd8e>	VMADN     vec22 = ( acc+= vec23 * vec30[2] )
	ErrorToCallFunction(0x04001C8C);

_04001C90:	// 4001c90: <c9101c00>	LDV       vec16 <08> = [T0 + 0x00]
	ErrorToCallFunction(0x04001C90);

_04001C94:	// 4001c94: <4bb5c504>	VMUDL     vec20 = ( acc = (vec24 * vec21[5] >> 16)      )
	ErrorToCallFunction(0x04001C94);

_04001C98:	// 4001c98: <ca6f1800>	LDV       vec15 <00> = [S3 + 0x00]
	ErrorToCallFunction(0x04001C98);

_04001C9C:	// 4001c9c: <c96e1800>	LDV       vec14 <00> = [T3 + 0x00]
	ErrorToCallFunction(0x04001C9C);

_04001CA0:	// 4001ca0: <c9ef1c00>	LDV       vec15 <08> = [T7 + 0x00]
	ErrorToCallFunction(0x04001CA0);

_04001CA4:	// 4001ca4: <c8ee1c00>	LDV       vec14 <08> = [A3 + 0x00]
	ErrorToCallFunction(0x04001CA4);

_04001CA8:	// 4001ca8: <4b9ea506>	VMUDN     vec20 = ( acc = vec20 * vec30[4]      ) >> 16
	ErrorToCallFunction(0x04001CA8);

_04001CAC:	// 4001cac: <ca4d1800>	LDV       vec13 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001CAC);

_04001CB0:	// 4001cb0: <4b75fd0e>	VMADN     vec20 = ( acc+= vec31 * vec21[3] )
	ErrorToCallFunction(0x04001CB0);

_04001CB4:	// 4001cb4: <c94c1800>	LDV       vec12 <00> = [T2 + 0x00]
	ErrorToCallFunction(0x04001CB4);

_04001CB8:	// 4001cb8: <c9cd1c00>	LDV       vec13 <08> = [T6 + 0x00]
	ErrorToCallFunction(0x04001CB8);

_04001CBC:	// 4001cbc: <4a129ac0>	VMULF     vec11 = ( acc = (vec19 * vec18[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CBC);

_04001CC0:	// 4001cc0: <c8cc1c00>	LDV       vec12 <08> = [A2 + 0x00]
	ErrorToCallFunction(0x04001CC0);

_04001CC4:	// 4001cc4: <4a108a80>	VMULF     vec10 = ( acc = (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CC4);

_04001CC8:	// 4001cc8: <e816207b>	SQV       [R0 + 0xb0] = vec22 <00>
	ErrorToCallFunction(0x04001CC8);

_04001CCC:	// 4001ccc: <4a0e7a40>	VMULF     vec09 = ( acc = (vec15 * vec14[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CCC);

_04001CD0:	// 4001cd0: <e814207c>	SQV       [R0 + 0xc0] = vec20 <00>
	ErrorToCallFunction(0x04001CD0);

_04001CD4:	// 4001cd4: <84150fb0>	LH        S5 = [R0+0fb0]
	ErrorToCallFunction(0x04001CD4);

_04001CD8:	// 4001cd8: <840d0fc0>	LH        T5 = [R0+0fc0]
	ErrorToCallFunction(0x04001CD8);

_04001CDC:	// 4001cdc: <4a0c6a00>	VMULF     vec08 = ( acc = (vec13 * vec12[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001CDC);

_04001CE0:	// 4001ce0: <84110fb8>	LH        S1 = [R0+0fb8]
	ErrorToCallFunction(0x04001CE0);

_04001CE4:	// 4001ce4: <4a6b5ad0>	VADD      vec11 = vec11 + vec11[1q]
	ErrorToCallFunction(0x04001CE4);

_04001CE8:	// 4001ce8: <84090fc8>	LH        T1 = [R0+0fc8]
	ErrorToCallFunction(0x04001CE8);

_04001CEC:	// 4001cec: <4a6a5290>	VADD      vec10 = vec10 + vec10[1q]
	ErrorToCallFunction(0x04001CEC);

_04001CF0:	// 4001cf0: <84140fb2>	LH        S4 = [R0+0fb2]
	ErrorToCallFunction(0x04001CF0);

_04001CF4:	// 4001cf4: <4a694a50>	VADD      vec09 = vec09 + vec09[1q]
	ErrorToCallFunction(0x04001CF4);

_04001CF8:	// 4001cf8: <840c0fc2>	LH        T4 = [R0+0fc2]
	ErrorToCallFunction(0x04001CF8);

_04001CFC:	// 4001cfc: <4a684210>	VADD      vec08 = vec08 + vec08[1q]
	ErrorToCallFunction(0x04001CFC);

_04001D00:	// 4001d00: <84100fba>	LH        S0 = [R0+0fba]
	ErrorToCallFunction(0x04001D00);

_04001D04:	// 4001d04: <4acb5ad0>	VADD      vec11 = vec11 + vec11[2h]
	ErrorToCallFunction(0x04001D04);

_04001D08:	// 4001d08: <84080fca>	LH        T0 = [R0+0fca]
	ErrorToCallFunction(0x04001D08);

_04001D0C:	// 4001d0c: <4aca5290>	VADD      vec10 = vec10 + vec10[2h]
	ErrorToCallFunction(0x04001D0C);

_04001D10:	// 4001d10: <84130fb4>	LH        S3 = [R0+0fb4]
	ErrorToCallFunction(0x04001D10);

_04001D14:	// 4001d14: <4ac94a50>	VADD      vec09 = vec09 + vec09[2h]
	ErrorToCallFunction(0x04001D14);

_04001D18:	// 4001d18: <840b0fc4>	LH        T3 = [R0+0fc4]
	ErrorToCallFunction(0x04001D18);

_04001D1C:	// 4001d1c: <4ac84210>	VADD      vec08 = vec08 + vec08[2h]
	ErrorToCallFunction(0x04001D1C);

_04001D20:	// 4001d20: <840f0fbc>	LH        T7 = [R0+0fbc]
	ErrorToCallFunction(0x04001D20);

_04001D24:	// 4001d24: <4a8be9c6>	VMUDN     vec07 = ( acc = vec29 * vec11[0h]      ) >> 16
	ErrorToCallFunction(0x04001D24);

_04001D28:	// 4001d28: <84070fcc>	LH        A3 = [R0+0fcc]
	ErrorToCallFunction(0x04001D28);

_04001D2C:	// 4001d2c: <4a8ae1ce>	VMADN     vec07 = ( acc+= vec28 * vec10[0h] )
	ErrorToCallFunction(0x04001D2C);

_04001D30:	// 4001d30: <84120fb6>	LH        S2 = [R0+0fb6]
	ErrorToCallFunction(0x04001D30);

_04001D34:	// 4001d34: <4a89d9ce>	VMADN     vec07 = ( acc+= vec27 * vec09[0h] )
	ErrorToCallFunction(0x04001D34);

_04001D38:	// 4001d38: <840a0fc6>	LH        T2 = [R0+0fc6]
	ErrorToCallFunction(0x04001D38);

_04001D3C:	// 4001d3c: <4a88d1ce>	VMADN     vec07 = ( acc+= vec26 * vec08[0h] )
	ErrorToCallFunction(0x04001D3C);

_04001D40:	// 4001d40: <840e0fbe>	LH        T6 = [R0+0fbe]
	ErrorToCallFunction(0x04001D40);

_04001D44:	// 4001d44: <84060fce>	LH        A2 = [R0+0fce]
	ErrorToCallFunction(0x04001D44);

_04001D48:	// 4001d48: <22d6fff0>	ADDI      S6 = S6 + fff0
	ErrorToCallFunction(0x04001D48);

_04001D4C:	// 4001d4c: <1ac00003>	BLEZ      (R0<=0) --> 1d5c
	sp_reg_pc = 0x04001D4C;
	if ((_s32)state.r[S6] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S7]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[7].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[7].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[7].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[7].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[7].U64[0] & temp1);
				value2 |= (state.v[7].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001D5C;
	}


_04001D50:	// 4001d50: <eae72000>	SQV       [S7 + 0x00] = vec07 <00>
	{
		_u32    addr = (0x00000000 + state.r[S7]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[7].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[7].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[7].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[7].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[7].U64[0] & temp1);
			value2 |= (state.v[7].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001D54:	// 4001d54: <09000717>	J         04001c5c
	state.r[S7] = state.r[S7] + 0x0010;
	goto _04001C5C;


_04001D58:	// 4001d58: <22f70010>	ADDI      S7 = S7 + 0010
	state.r[S7] = state.r[S7] + 0x0010;


_04001D5C:	// 4001d5c: <cab31800>	LDV       vec19 <00> = [S5 + 0x00]
	rsp_ldv_compiler((state.r[S5] & 0xFFF), 19, 8);


_04001D60:	// 4001d60: <eb180804>	SSV       [T8 + 0x08] = vec24 <00>
	Save16_DMEM(state.v[24].U16[7], (0x00000008 + state.r[T8]) & 0xfff);


_04001D64:	// 4001d64: <0d000466>	JAL	    04001198
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}
	sp_reg_pc = 0x04001D64;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001D68:	// 4001d68: <eb131800>	SDV       [T8 + 0x00] = vec19 <00>
	{
		_u32	addr = (0x00000000 + state.r[T8]);

		if( addr & 3 )
		{
			Save64_DMEM(state.v[19].U64[0x00000001], addr);
		}
		else
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+4] = state.v[19].U32[0x00000002];
			*(_u32 *)&pDMEM[addr  ] = state.v[19].U32[0x00000003];
		}
	}


_04001D6C:	// 4001d6c: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001D70:	// 4001d70: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001D74:	// 4001d74: <14a0ffff>	BNE       (R0!=A1) --> 1d74
	sp_reg_pc = 0x04001D74;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001D74;
	}


_04001D78:	// 4001d78: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001D7C:	// 4001d7c: <09000443>	J         0400110c
	goto _0400110C;


_04001D80:	// 4001d80: <00000000>	NOP       


_04001D84:	// 4001d84: <00191200>	SLL       V0 = T9 << 8
	state.r[V0] = ((_u32)state.r[T9] << 8);


_04001D88:	// 4001d88: <00021202>	SRL       V0 = V0 >> 8
	state.r[V0] = ((_u32)state.r[V0] >> 8);


_04001D8C:	// 4001d8c: <c81f2005>	LQV       vec31 <00> = [R0 + 0x50]
	{
		_u32 addr = (0x00000050 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[31].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[31].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[31].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[31].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[31].U64[0] = state.v[31].U64[0] & ~temp1;
			state.v[31].U64[1] = state.v[31].U64[1] & ~temp2;
			state.v[31].U64[0] = state.v[31].U64[0] | value1;
			state.v[31].U64[1] = state.v[31].U64[1] | value2;
		}
	}


_04001D90:	// 4001d90: <c80a2006>	LQV       vec10 <00> = [R0 + 0x60]
	{
		_u32 addr = (0x00000060 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[10].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[10].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[10].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[10].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[10].U64[0] = state.v[10].U64[0] & ~temp1;
			state.v[10].U64[1] = state.v[10].U64[1] & ~temp2;
			state.v[10].U64[0] = state.v[10].U64[0] | value1;
			state.v[10].U64[1] = state.v[10].U64[1] | value2;
		}
	}


_04001D94:	// 4001d94: <c81e200b>	LQV       vec30 <00> = [R0 + 0xb0]
	{
		_u32 addr = (0x000000B0 + 0);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[30].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[30].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[30].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[30].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[30].U64[0] = state.v[30].U64[0] & ~temp1;
			state.v[30].U64[1] = state.v[30].U64[1] & ~temp2;
			state.v[30].U64[0] = state.v[30].U64[0] | value1;
			state.v[30].U64[1] = state.v[30].U64[1] | value2;
		}
	}


_04001D98:	// 4001d98: <4a00002c>	VXOR      vec00 = vec00 xor vec00[<none>]
	rsp_cop2_vxor(&state.v[0], &state.v[0], &state.v[0]);


_04001D9C:	// 4001d9c: <001a7402>	SRL       T6 = K0 >> 16
	state.r[T6] = ((_u32)state.r[K0] >> 16);


_04001DA0:	// 4001da0: <31cf0001>	ANDI      T7 = T6 & 0001
	state.r[T7] = (state.r[T6] & 0x00000001);


_04001DA4:	// 4001da4: <1de0000a>	BGTZ      (R0>0) --> 1dd0
	sp_reg_pc = 0x04001DA4;
	if((_s32)state.r[T7] > 0)
	{
		state.r[AT] = state.r[T8] + 0x0000;
		goto _04001DD0;
	}


_04001DA8:	// 4001da8: <23010000>	ADDI      AT = T8 + 0000
	state.r[AT] = state.r[T8] + 0x0000;


_04001DAC:	// 4001dac: <0d00045f>	JAL	    0400117c
	state.r[V1] = 0 + 0x004F;
	sp_reg_pc = 0x04001DAC;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _0400117C;


_04001DB0:	// 4001db0: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001DB4:	// 4001db4: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001DB8:	// 4001db8: <14a0ffff>	BNE       (R0!=A1) --> 1db8
	sp_reg_pc = 0x04001DB8;
	if (state.r[A1] != 0)
	{
		state.r[A1] = 0;
		goto _04001DB8;
	}


_04001DBC:	// 4001dbc: <40053000>	MFC0      A2 = SP DMA full
	state.r[A1] = 0;


_04001DC0:	// 4001dc0: <cb142000>	LQV       vec20 <00> = [T8 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[20].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[20].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[20].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[20].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[20].U64[0] = state.v[20].U64[0] & ~temp1;
			state.v[20].U64[1] = state.v[20].U64[1] & ~temp2;
			state.v[20].U64[0] = state.v[20].U64[0] | value1;
			state.v[20].U64[1] = state.v[20].U64[1] | value2;
		}
	}


_04001DC4:	// 4001dc4: <cb152001>	LQV       vec21 <00> = [T8 + 0x10]
	{
		_u32 addr = (0x00000010 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[21].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[21].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[21].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[21].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[21].U64[0] = state.v[21].U64[0] & ~temp1;
			state.v[21].U64[1] = state.v[21].U64[1] & ~temp2;
			state.v[21].U64[0] = state.v[21].U64[0] | value1;
			state.v[21].U64[1] = state.v[21].U64[1] | value2;
		}
	}


_04001DC8:	// 4001dc8: <cb122002>	LQV       vec18 <00> = [T8 + 0x20]
	{
		_u32 addr = (0x00000020 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[18].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[18].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[18].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[18].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[18].U64[0] = state.v[18].U64[0] & ~temp1;
			state.v[18].U64[1] = state.v[18].U64[1] & ~temp2;
			state.v[18].U64[0] = state.v[18].U64[0] | value1;
			state.v[18].U64[1] = state.v[18].U64[1] | value2;
		}
	}


_04001DCC:	// 4001dcc: <cb132003>	LQV       vec19 <00> = [T8 + 0x30]
	{
		_u32 addr = (0x00000030 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[19].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[19].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[19].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[19].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[19].U64[0] = state.v[19].U64[0] & ~temp1;
			state.v[19].U64[1] = state.v[19].U64[1] & ~temp2;
			state.v[19].U64[0] = state.v[19].U64[0] | value1;
			state.v[19].U64[1] = state.v[19].U64[1] | value2;
		}
	}


_04001DD0:	// 4001dd0: <cb182004>	LQV       vec24 <00> = [T8 + 0x40]
	{
		_u32 addr = (0x00000040 + state.r[T8]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[24].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[24].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[24].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[24].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[24].U64[0] = state.v[24].U64[0] & ~temp1;
			state.v[24].U64[1] = state.v[24].U64[1] & ~temp2;
			state.v[24].U64[0] = state.v[24].U64[0] | value1;
			state.v[24].U64[1] = state.v[24].U64[1] | value2;
		}
	}


_04001DD4:	// 4001dd4: <20100500>	ADDI      S0 = R0 + 0500
	state.r[S0] = 0 + 0x0500;


_04001DD8:	// 4001dd8: <201509e0>	ADDI      S5 = R0 + 09e0
	state.r[S5] = 0 + 0x09E0;


_04001DDC:	// 4001ddc: <20140b50>	ADDI      S4 = R0 + 0b50
	state.r[S4] = 0 + 0x0B50;


_04001DE0:	// 4001de0: <20130cc0>	ADDI      S3 = R0 + 0cc0
	state.r[S3] = 0 + 0x0CC0;


_04001DE4:	// 4001de4: <20120e30>	ADDI      S2 = R0 + 0e30
	state.r[S2] = 0 + 0x0E30;


_04001DE8:	// 4001de8: <20110170>	ADDI      S1 = R0 + 0170
	state.r[S1] = 0 + 0x0170;


_04001DEC:	// 4001dec: <4816c400>	MFC2      S6 = vec24 <08>
	state.r[S6] = (_s32)state.v[24].S16[3];


_04001DF0:	// 4001df0: <4bd8fa68>	VAND      vec09 = vec31 and vec24[6]
	rsp_cop2_vand_element(&state.v[9], &state.v[31], &state.v[24].U16[1]);


_04001DF4:	// 4001df4: <4bf8fa28>	VAND      vec08 = vec31 and vec24[7]
	rsp_cop2_vand_element(&state.v[8], &state.v[31], &state.v[24].U16[0]);


_04001DF8:	// 4001df8: <4a090251>	VSUB      vec09 = vec00 - vec09[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[9].U16[0], state.v[0].U16[0], state.v[9].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[9].U16[1], state.v[0].U16[1], state.v[9].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[9].U16[2], state.v[0].U16[2], state.v[9].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[9].U16[3], state.v[0].U16[3], state.v[9].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[9].U16[4], state.v[0].U16[4], state.v[9].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[9].U16[5], state.v[0].U16[5], state.v[9].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[9].U16[6], state.v[0].U16[6], state.v[9].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[9].U16[7], state.v[0].U16[7], state.v[9].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001DFC:	// 4001dfc: <4a080211>	VSUB      vec08 = vec00 - vec08[<none>]
	{
		_u16		flag0 = state.flag[0];
		VSUB_operation(0, state.v[8].U16[0], state.v[0].U16[0], state.v[8].U16[0], 1, 0, 0, 1)
		VSUB_operation(1, state.v[8].U16[1], state.v[0].U16[1], state.v[8].U16[1], 1, 0, 0, 1)
		VSUB_operation(2, state.v[8].U16[2], state.v[0].U16[2], state.v[8].U16[2], 1, 0, 0, 1)
		VSUB_operation(3, state.v[8].U16[3], state.v[0].U16[3], state.v[8].U16[3], 1, 0, 0, 1)
		VSUB_operation(4, state.v[8].U16[4], state.v[0].U16[4], state.v[8].U16[4], 1, 0, 0, 1)
		VSUB_operation(5, state.v[8].U16[5], state.v[0].U16[5], state.v[8].U16[5], 1, 0, 0, 1)
		VSUB_operation(6, state.v[8].U16[6], state.v[0].U16[6], state.v[8].U16[6], 1, 0, 0, 1)
		VSUB_operation(7, state.v[8].U16[7], state.v[0].U16[7], state.v[8].U16[7], 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001E00:	// 4001e00: <4a09422c>	VXOR      vec08 = vec08 xor vec09[<none>]
	rsp_cop2_vxor(&state.v[8], &state.v[8], &state.v[9]);


_04001E04:	// 4001e04: <11e00035>	BEQ       (R0==T7) --> 1edc
	sp_reg_pc = 0x04001E04;
	if (state.r[T7] == 0)
	{
		state.r[S7] = (_s32)state.v[24].S16[6];
		goto _04001EDC;
	}


_04001E08:	// 4001e08: <4817c100>	MFC2      S7 = vec24 <02>
	state.r[S7] = (_s32)state.v[24].S16[6];


_04001E0C:	// 4001e0c: <2003004f>	ADDI      V1 = R0 + 004f
	state.r[V1] = 0 + 0x004F;


_04001E10:	// 4001e10: <4a14a52c>	VXOR      vec20 = vec20 xor vec20[<none>]
	rsp_cop2_vxor(&state.v[20], &state.v[20], &state.v[20]);


_04001E14:	// 4001e14: <cb140f28>	LSV       vec20 <14> = [T8 + 0x10]
	state.v[20].U16[0] = Load16_DMEM(0x00000050 + state.r[T8]);


_04001E18:	// 4001e18: <4a15ad6c>	VXOR      vec21 = vec21 xor vec21[<none>]
	rsp_cop2_vxor(&state.v[21], &state.v[21], &state.v[21]);


_04001E1C:	// 4001e1c: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001E20:	// 4001e20: <4a1294ac>	VXOR      vec18 = vec18 xor vec18[<none>]
	rsp_cop2_vxor(&state.v[18], &state.v[18], &state.v[18]);


_04001E24:	// 4001e24: <489a9700>	MTC2      vec18 <14> = K0
	state.v[18].U16[0] = (_u16)state.r[K0];


_04001E28:	// 4001e28: <4b58f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[2] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[5];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E2C:	// 4001e2c: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001E30:	// 4001e30: <4b38f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[1] )
	{
		_u16		s2value = state.v[24].U16[6];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E34:	// 4001e34: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001E38:	// 4001e38: <4bf4fd0f>	VMADH     vec20 = ( acc+= (vec31 * vec20[7]) << 16) >> 16
	{
		_u16		s2value = state.v[20].U16[0];
		VMADH_operation(0, state.v[20].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[20].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[20].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[20].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[20].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[20].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[20].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[20].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001E3C:	// 4001e3c: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S4]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[28].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[28].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[28].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[28].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[28].U64[0] = state.v[28].U64[0] & ~temp1;
			state.v[28].U64[1] = state.v[28].U64[1] & ~temp2;
			state.v[28].U64[0] = state.v[28].U64[0] | value1;
			state.v[28].U64[1] = state.v[28].U64[1] | value2;
		}
	}


_04001E40:	// 4001e40: <4b00fd4e>	VMADN     vec21 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[21].U16[0], state.v[31].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[21].U16[1], state.v[31].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[21].U16[2], state.v[31].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[21].U16[3], state.v[31].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[21].U16[4], state.v[31].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[21].U16[5], state.v[31].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[21].U16[6], state.v[31].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[21].U16[7], state.v[31].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E44:	// 4001e44: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001E44;
	if (state.r[S7] >= 0)
	{
		rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);
		goto _04001E54;
	}


_04001E48:	// 4001e48: <4a139cec>	VXOR      vec19 = vec19 xor vec19[<none>]
	rsp_cop2_vxor(&state.v[19], &state.v[19], &state.v[19]);


_04001E4C:	// 4001e4c: <09000796>	J         04001e58
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001E58;


_04001E50:	// 4001e50: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001E54:	// 4001e54: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001E58:	// 4001e58: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001E5C:	// 4001e5c: <4bb8f5c4>	VMUDL     vec23 = ( acc = (vec30 * vec24[5] >> 16)      )
	{
		_u16		s2value = state.v[24].U16[2];
		VMUDL_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 0, 1, 1, 1)
		VMUDL_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 0, 1, 1, 1)
		VMUDL_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 0, 1, 1, 1)
		VMUDL_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 0, 1, 1, 1)
		VMUDL_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 0, 1, 1, 1)
		VMUDL_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 0, 1, 1, 1)
		VMUDL_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 0, 1, 1, 1)
		VMUDL_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E60:	// 4001e60: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S2]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[26].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[26].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[26].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[26].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[26].U64[0] = state.v[26].U64[0] & ~temp1;
			state.v[26].U64[1] = state.v[26].U64[1] & ~temp2;
			state.v[26].U64[0] = state.v[26].U64[0] | value1;
			state.v[26].U64[1] = state.v[26].U64[1] | value2;
		}
	}


_04001E64:	// 4001e64: <4b98f5ce>	VMADN     vec23 = ( acc+= vec30 * vec24[4] )
	{
		_u16		s2value = state.v[24].U16[3];
		VMADN_operation(0, state.v[23].U16[0], state.v[30].U16[0], s2value, 1, 1, 1, 1)
		VMADN_operation(1, state.v[23].U16[1], state.v[30].U16[1], s2value, 1, 1, 1, 1)
		VMADN_operation(2, state.v[23].U16[2], state.v[30].U16[2], s2value, 1, 1, 1, 1)
		VMADN_operation(3, state.v[23].U16[3], state.v[30].U16[3], s2value, 1, 1, 1, 1)
		VMADN_operation(4, state.v[23].U16[4], state.v[30].U16[4], s2value, 1, 1, 1, 1)
		VMADN_operation(5, state.v[23].U16[5], state.v[30].U16[5], s2value, 1, 1, 1, 1)
		VMADN_operation(6, state.v[23].U16[6], state.v[30].U16[6], s2value, 1, 1, 1, 1)
		VMADN_operation(7, state.v[23].U16[7], state.v[30].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E68:	// 4001e68: <2231fff0>	ADDI      S1 = S1 + fff0
	state.r[S1] = state.r[S1] + 0xFFFFFFF0;


_04001E6C:	// 4001e6c: <4bf2fc8f>	VMADH     vec18 = ( acc+= (vec31 * vec18[7]) << 16) >> 16
	{
		_u16		s2value = state.v[18].U16[0];
		VMADH_operation(0, state.v[18].U16[0], state.v[31].U16[0], s2value, 1, 1, 0, 1)
		VMADH_operation(1, state.v[18].U16[1], state.v[31].U16[1], s2value, 1, 1, 0, 1)
		VMADH_operation(2, state.v[18].U16[2], state.v[31].U16[2], s2value, 1, 1, 0, 1)
		VMADH_operation(3, state.v[18].U16[3], state.v[31].U16[3], s2value, 1, 1, 0, 1)
		VMADH_operation(4, state.v[18].U16[4], state.v[31].U16[4], s2value, 1, 1, 0, 1)
		VMADH_operation(5, state.v[18].U16[5], state.v[31].U16[5], s2value, 1, 1, 0, 1)
		VMADH_operation(6, state.v[18].U16[6], state.v[31].U16[6], s2value, 1, 1, 0, 1)
		VMADH_operation(7, state.v[18].U16[7], state.v[31].U16[7], s2value, 1, 1, 0, 1)
	}


_04001E70:	// 4001e70: <22100010>	ADDI      S0 = S0 + 0010
	state.r[S0] = state.r[S0] + 0x0010;


_04001E74:	// 4001e74: <4b00fcce>	VMADN     vec19 = ( acc+= vec31 * vec00[0] )
	{
		_u16		s2value = state.v[0].U16[7];
		VMADN_operation(0, state.v[19].U16[0], state.v[31].U16[0], s2value, 1, 0, 0, 1)
		VMADN_operation(1, state.v[19].U16[1], state.v[31].U16[1], s2value, 1, 0, 0, 1)
		VMADN_operation(2, state.v[19].U16[2], state.v[31].U16[2], s2value, 1, 0, 0, 1)
		VMADN_operation(3, state.v[19].U16[3], state.v[31].U16[3], s2value, 1, 0, 0, 1)
		VMADN_operation(4, state.v[19].U16[4], state.v[31].U16[4], s2value, 1, 0, 0, 1)
		VMADN_operation(5, state.v[19].U16[5], state.v[31].U16[5], s2value, 1, 0, 0, 1)
		VMADN_operation(6, state.v[19].U16[6], state.v[31].U16[6], s2value, 1, 0, 0, 1)
		VMADN_operation(7, state.v[19].U16[7], state.v[31].U16[7], s2value, 1, 0, 0, 1)
	}


_04001E78:	// 4001e78: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E7C:	// 4001e7c: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001E7C;
	if (state.r[S6] >= 0)
	{
		{
			_u16		s2value = state.v[24].U16[0];
			VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
			VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
			VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
			VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
			VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
			VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
			VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
			VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
		}
		goto _04001E8C;
	}


_04001E80:	// 4001e80: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[20].U16[0], s2value, 1, 1, 1, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[20].U16[1], s2value, 1, 1, 1, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[20].U16[2], s2value, 1, 1, 1, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[20].U16[3], s2value, 1, 1, 1, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[20].U16[4], s2value, 1, 1, 1, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[20].U16[5], s2value, 1, 1, 1, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[20].U16[6], s2value, 1, 1, 1, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[20].U16[7], s2value, 1, 1, 1, 1)
	}


_04001E84:	// 4001e84: <090007a4>	J         04001e90
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001E90;


_04001E88:	// 4001e88: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001E8C:	// 4001e8c: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001E90:	// 4001e90: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[29].U16[0], state.v[29].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[29].U16[1], state.v[29].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[29].U16[2], state.v[29].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[29].U16[3], state.v[29].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[29].U16[4], state.v[29].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[29].U16[5], state.v[29].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[29].U16[6], state.v[29].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[29].U16[7], state.v[29].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E94:	// 4001e94: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[29].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[29].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[29].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[29].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[29].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[29].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[29].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[29].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001E98:	// 4001e98: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[27].U16[0], state.v[27].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[27].U16[1], state.v[27].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[27].U16[2], state.v[27].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[27].U16[3], state.v[27].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[27].U16[4], state.v[27].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[27].U16[5], state.v[27].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[27].U16[6], state.v[27].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[27].U16[7], state.v[27].U16[7], s2value, 0, 1, 1, 1)
	}


_04001E9C:	// 4001e9c: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[27].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[27].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[27].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[27].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[27].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[27].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[27].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[27].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 0, 0, 1)
	}


_04001EA0:	// 4001ea0: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[8], &state.v[17]);


_04001EA4:	// 4001ea4: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001EA8:	// 4001ea8: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[0];
		VMULF_operation(0, state.v[15].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[15].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[15].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[15].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[15].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[15].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[15].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[15].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
	}


_04001EAC:	// 4001eac: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	{
		_u32    addr = (0x00000000 + state.r[S5]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[29].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[29].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[29].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[29].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[29].U64[0] & temp1);
			value2 |= (state.v[29].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001EB0:	// 4001eb0: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[28].U16[0], state.v[28].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[28].U16[1], state.v[28].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[28].U16[2], state.v[28].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[28].U16[3], state.v[28].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[28].U16[4], state.v[28].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[28].U16[5], state.v[28].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[28].U16[6], state.v[28].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[28].U16[7], state.v[28].U16[7], s2value, 0, 1, 1, 1)
	}


_04001EB4:	// 4001eb4: <22b50010>	ADDI      S5 = S5 + 0010
	state.r[S5] = state.r[S5] + 0x0010;


_04001EB8:	// 4001eb8: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[28].U16[0], state.v[17].U16[0], state.v[16].U16[0], 1, 0, 0, 1)
		VMACF_operation(1, state.v[28].U16[1], state.v[17].U16[1], state.v[16].U16[1], 1, 0, 0, 1)
		VMACF_operation(2, state.v[28].U16[2], state.v[17].U16[2], state.v[16].U16[2], 1, 0, 0, 1)
		VMACF_operation(3, state.v[28].U16[3], state.v[17].U16[3], state.v[16].U16[3], 1, 0, 0, 1)
		VMACF_operation(4, state.v[28].U16[4], state.v[17].U16[4], state.v[16].U16[4], 1, 0, 0, 1)
		VMACF_operation(5, state.v[28].U16[5], state.v[17].U16[5], state.v[16].U16[5], 1, 0, 0, 1)
		VMACF_operation(6, state.v[28].U16[6], state.v[17].U16[6], state.v[16].U16[6], 1, 0, 0, 1)
		VMACF_operation(7, state.v[28].U16[7], state.v[17].U16[7], state.v[16].U16[7], 1, 0, 0, 1)
	}


_04001EBC:	// 4001ebc: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	{
		_u32    addr = (0x00000000 + state.r[S3]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[27].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[27].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[27].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[27].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[27].U64[0] & temp1);
			value2 |= (state.v[27].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001EC0:	// 4001ec0: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	{
		_u16		s2value = state.v[10].U16[2];
		VMULF_operation(0, state.v[26].U16[0], state.v[26].U16[0], s2value, 0, 1, 1, 1)
		VMULF_operation(1, state.v[26].U16[1], state.v[26].U16[1], s2value, 0, 1, 1, 1)
		VMULF_operation(2, state.v[26].U16[2], state.v[26].U16[2], s2value, 0, 1, 1, 1)
		VMULF_operation(3, state.v[26].U16[3], state.v[26].U16[3], s2value, 0, 1, 1, 1)
		VMULF_operation(4, state.v[26].U16[4], state.v[26].U16[4], s2value, 0, 1, 1, 1)
		VMULF_operation(5, state.v[26].U16[5], state.v[26].U16[5], s2value, 0, 1, 1, 1)
		VMULF_operation(6, state.v[26].U16[6], state.v[26].U16[6], s2value, 0, 1, 1, 1)
		VMULF_operation(7, state.v[26].U16[7], state.v[26].U16[7], s2value, 0, 1, 1, 1)
	}


_04001EC4:	// 4001ec4: <22730010>	ADDI      S3 = S3 + 0010
	state.r[S3] = state.r[S3] + 0x0010;


_04001EC8:	// 4001ec8: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	{
		VMACF_operation(0, state.v[26].U16[0], state.v[17].U16[0], state.v[15].U16[0], 1, 1, 1, 1)
		VMACF_operation(1, state.v[26].U16[1], state.v[17].U16[1], state.v[15].U16[1], 1, 1, 1, 1)
		VMACF_operation(2, state.v[26].U16[2], state.v[17].U16[2], state.v[15].U16[2], 1, 1, 1, 1)
		VMACF_operation(3, state.v[26].U16[3], state.v[17].U16[3], state.v[15].U16[3], 1, 1, 1, 1)
		VMACF_operation(4, state.v[26].U16[4], state.v[17].U16[4], state.v[15].U16[4], 1, 1, 1, 1)
		VMACF_operation(5, state.v[26].U16[5], state.v[17].U16[5], state.v[15].U16[5], 1, 1, 1, 1)
		VMACF_operation(6, state.v[26].U16[6], state.v[17].U16[6], state.v[15].U16[6], 1, 1, 1, 1)
		VMACF_operation(7, state.v[26].U16[7], state.v[17].U16[7], state.v[15].U16[7], 1, 1, 1, 1)
	}


_04001ECC:	// 4001ecc: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	{
		_u32    addr = (0x00000000 + state.r[S4]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[28].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[28].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[28].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[28].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[28].U64[0] & temp1);
			value2 |= (state.v[28].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001ED0:	// 4001ed0: <22940010>	ADDI      S4 = S4 + 0010
	state.r[S4] = state.r[S4] + 0x0010;


_04001ED4:	// 4001ed4: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001ED8:	// 4001ed8: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001EDC:	// 4001edc: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[5];
		VADDC_operation(0, state.v[21].U16[0], state.v[21].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[21].U16[1], state.v[21].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[21].U16[2], state.v[21].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[21].U16[3], state.v[21].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[21].U16[4], state.v[21].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[21].U16[5], state.v[21].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[21].U16[6], state.v[21].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[21].U16[7], state.v[21].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001EE0:	// 4001ee0: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[6];
		VADD_operation(0, state.v[20].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[20].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[20].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[20].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[20].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[20].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[20].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[20].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001EE4:	// 4001ee4: <cabd2000>	LQV       vec29 <00> = [S5 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S5]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[29].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[29].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[29].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[29].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[29].U64[0] = state.v[29].U64[0] & ~temp1;
			state.v[29].U64[1] = state.v[29].U64[1] & ~temp2;
			state.v[29].U64[0] = state.v[29].U64[0] | value1;
			state.v[29].U64[1] = state.v[29].U64[1] | value2;
		}
	}


_04001EE8:	// 4001ee8: <4bb89cd4>	VADDC	vec19 = vec19 + vec24[5]
	{
		_u16		flag0 = 0;
		_u16		s2value = state.v[24].U16[2];
		VADDC_operation(0, state.v[19].U16[0], state.v[19].U16[0], s2value, 1, 0, 0, 1)
		VADDC_operation(1, state.v[19].U16[1], state.v[19].U16[1], s2value, 1, 0, 0, 1)
		VADDC_operation(2, state.v[19].U16[2], state.v[19].U16[2], s2value, 1, 0, 0, 1)
		VADDC_operation(3, state.v[19].U16[3], state.v[19].U16[3], s2value, 1, 0, 0, 1)
		VADDC_operation(4, state.v[19].U16[4], state.v[19].U16[4], s2value, 1, 0, 0, 1)
		VADDC_operation(5, state.v[19].U16[5], state.v[19].U16[5], s2value, 1, 0, 0, 1)
		VADDC_operation(6, state.v[19].U16[6], state.v[19].U16[6], s2value, 1, 0, 0, 1)
		VADDC_operation(7, state.v[19].U16[7], state.v[19].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = flag0;
	}


_04001EEC:	// 4001eec: <ca112000>	LQV       vec17 <00> = [S0 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S0]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[17].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[17].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[17].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[17].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[17].U64[0] = state.v[17].U64[0] & ~temp1;
			state.v[17].U64[1] = state.v[17].U64[1] & ~temp2;
			state.v[17].U64[0] = state.v[17].U64[0] | value1;
			state.v[17].U64[1] = state.v[17].U64[1] | value2;
		}
	}


_04001EF0:	// 4001ef0: <06e10003>	BGEZ ?-?
	sp_reg_pc = 0x04001EF0;
	if (state.r[S7] >= 0)
	{
		{
			_u16		flag0 = state.flag[0];
			_u16		s2value = state.v[24].U16[3];
			VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 1, 1, 1)
			VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 1, 1, 1)
			VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 1, 1, 1)
			VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 1, 1, 1)
			VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 1, 1, 1)
			VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 1, 1, 1)
			VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 1, 1, 1)
			VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 1, 1, 1)
			state.flag[0] = 0x0000;
		}
		goto _04001F00;
	}


_04001EF4:	// 4001ef4: <4b989490>	VADD      vec18 = vec18 + vec24[4]
	{
		_u16		flag0 = state.flag[0];
		_u16		s2value = state.v[24].U16[3];
		VADD_operation(0, state.v[18].U16[0], state.v[18].U16[0], s2value, 1, 0, 0, 1)
		VADD_operation(1, state.v[18].U16[1], state.v[18].U16[1], s2value, 1, 0, 0, 1)
		VADD_operation(2, state.v[18].U16[2], state.v[18].U16[2], s2value, 1, 0, 0, 1)
		VADD_operation(3, state.v[18].U16[3], state.v[18].U16[3], s2value, 1, 0, 0, 1)
		VADD_operation(4, state.v[18].U16[4], state.v[18].U16[4], s2value, 1, 0, 0, 1)
		VADD_operation(5, state.v[18].U16[5], state.v[18].U16[5], s2value, 1, 0, 0, 1)
		VADD_operation(6, state.v[18].U16[6], state.v[18].U16[6], s2value, 1, 0, 0, 1)
		VADD_operation(7, state.v[18].U16[7], state.v[18].U16[7], s2value, 1, 0, 0, 1)
		state.flag[0] = 0x0000;
	}


_04001EF8:	// 4001ef8: <090007c1>	J         04001f04
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);
	goto _04001F04;


_04001EFC:	// 4001efc: <4b18a523>	VGE       vec20 = (vec20 >= vec24[0])
	rsp_cop2_vge_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001F00:	// 4001f00: <4b18a520>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[20], &state.v[20], &state.v[24].U16[7]);


_04001F04:	// 4001f04: <4a114c6c>	VXOR      vec17 = vec09 xor vec17[<none>]
	rsp_cop2_vxor(&state.v[17], &state.v[9], &state.v[17]);


_04001F08:	// 4001f08: <06c10003>	BGEZ ?-?
	sp_reg_pc = 0x04001F08;
	if (state.r[S6] >= 0)
	{
		{
			_u32 addr = (0x00000000 + state.r[S3]);
			int shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
				state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
				state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
				state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
			}
			else
			{
				_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
				_u64		value1 = Load64_DMEM(addr+8);
				_u64		value2 = Load64_DMEM(addr);
				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & temp1;
				value2 = value2 & temp2;

				state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
				state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
				state.v[27].U64[0] = state.v[27].U64[0] | value1;
				state.v[27].U64[1] = state.v[27].U64[1] | value2;
			}
		}
		goto _04001F18;
	}


_04001F0C:	// 4001f0c: <ca7b2000>	LQV       vec27 <00> = [S3 + 0x00]
	{
		_u32 addr = (0x00000000 + state.r[S3]);
		int shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			state.v[27].U32[0] = *(_u32 *)&pDMEM[addr+12];
			state.v[27].U32[1] = *(_u32 *)&pDMEM[addr+8];
			state.v[27].U32[2] = *(_u32 *)&pDMEM[addr+4];
			state.v[27].U32[3] = *(_u32 *)&pDMEM[addr];
		}
		else
		{
			_u64		temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64		temp2 = 0xFFFFFFFFFFFFFFFF;
			_u64		value1 = Load64_DMEM(addr+8);
			_u64		value2 = Load64_DMEM(addr);
			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & temp1;
			value2 = value2 & temp2;

			state.v[27].U64[0] = state.v[27].U64[0] & ~temp1;
			state.v[27].U64[1] = state.v[27].U64[1] & ~temp2;
			state.v[27].U64[0] = state.v[27].U64[0] | value1;
			state.v[27].U64[1] = state.v[27].U64[1] | value2;
		}
	}


_04001F10:	// 4001f10: <090007c7>	J         04001f1c
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);
	goto _04001F1C;


_04001F14:	// 4001f14: <4b7894a3>	VGE       vec18 = (vec18 >= vec24[3])
	rsp_cop2_vge_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001F18:	// 4001f18: <4b7894a0>	VLT ?-?
	rsp_cop2_vlt_element(&state.v[18], &state.v[18], &state.v[24].U16[4]);


_04001F1C:	// 4001f1c: <4bd8a400>	VMULF     vec16 = ( acc = (vec20 * vec24[6]) << 1 ) >> 16
	{
		_u16		s2value = state.v[24].U16[1];
		VMULF_operation(0, state.v[16].U16[0], state.v[20].U16[0], s2value, 1, 0, 0, 1)
		VMULF_operation(1, state.v[16].U16[1], state.v[20].U16[1], s2value, 1, 0, 0, 1)
		VMULF_operation(2, state.v[16].U16[2], state.v[20].U16[2], s2value, 1, 0, 0, 1)
		VMULF_operation(3, state.v[16].U16[3], state.v[20].U16[3], s2value, 1, 0, 0, 1)
		VMULF_operation(4, state.v[16].U16[4], state.v[20].U16[4], s2value, 1, 0, 0, 1)
		VMULF_operation(5, state.v[16].U16[5], state.v[20].U16[5], s2value, 1, 0, 0, 1)
		VMULF_operation(6, state.v[16].U16[6], state.v[20].U16[6], s2value, 1, 0, 0, 1)
		VMULF_operation(7, state.v[16].U16[7], state.v[20].U16[7], s2value, 1, 0, 0, 1)
	}


_04001F20:	// 4001f20: <eb142000>	SQV       [T8 + 0x00] = vec20 <00>
	// In function: Tooie1F20_1F84
	//Call the function: Tooie1F20_1F84
	Tooie1F20_1F84();
	goto _04001F88;

_04001F24:	// 4001f24: <4bf8a3c0>	VMULF     vec15 = ( acc = (vec20 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F24);

_04001F28:	// 4001f28: <eb152001>	SQV       [T8 + 0x10] = vec21 <00>
	ErrorToCallFunction(0x04001F28);

_04001F2C:	// 4001f2c: <4baaef40>	VMULF     vec29 = ( acc = (vec29 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F2C);

_04001F30:	// 4001f30: <4a108f48>	VMACF     vec29 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F30);

_04001F34:	// 4001f34: <ca9c2000>	LQV       vec28 <00> = [S4 + 0x00]
	ErrorToCallFunction(0x04001F34);

_04001F38:	// 4001f38: <4baadec0>	VMULF     vec27 = ( acc = (vec27 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F38);

_04001F3C:	// 4001f3c: <ca5a2000>	LQV       vec26 <00> = [S2 + 0x00]
	ErrorToCallFunction(0x04001F3C);

_04001F40:	// 4001f40: <4a0f8ec8>	VMACF     vec27 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F40);

_04001F44:	// 4001f44: <4a11446c>	VXOR      vec17 = vec08 xor vec17[<none>]
	ErrorToCallFunction(0x04001F44);

_04001F48:	// 4001f48: <2231fff0>	ADDI      S1 = S1 + fff0
	ErrorToCallFunction(0x04001F48);

_04001F4C:	// 4001f4c: <4b58ad54>	VADDC	vec21 = vec21 + vec24[2]
	ErrorToCallFunction(0x04001F4C);

_04001F50:	// 4001f50: <22100010>	ADDI      S0 = S0 + 0010
	ErrorToCallFunction(0x04001F50);

_04001F54:	// 4001f54: <4b38a510>	VADD      vec20 = vec20 + vec24[1]
	ErrorToCallFunction(0x04001F54);

_04001F58:	// 4001f58: <eabd2000>	SQV       [S5 + 0x00] = vec29 <00>
	ErrorToCallFunction(0x04001F58);

_04001F5C:	// 4001f5c: <4bd89400>	VMULF     vec16 = ( acc = (vec18 * vec24[6]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F5C);

_04001F60:	// 4001f60: <22b50010>	ADDI      S5 = S5 + 0010
	ErrorToCallFunction(0x04001F60);

_04001F64:	// 4001f64: <4bf893c0>	VMULF     vec15 = ( acc = (vec18 * vec24[7]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F64);

_04001F68:	// 4001f68: <ea7b2000>	SQV       [S3 + 0x00] = vec27 <00>
	ErrorToCallFunction(0x04001F68);

_04001F6C:	// 4001f6c: <4baae700>	VMULF     vec28 = ( acc = (vec28 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F6C);

_04001F70:	// 4001f70: <22730010>	ADDI      S3 = S3 + 0010
	ErrorToCallFunction(0x04001F70);

_04001F74:	// 4001f74: <4a108f08>	VMACF     vec28 = ( acc += (vec17 * vec16[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F74);

_04001F78:	// 4001f78: <4baad680>	VMULF     vec26 = ( acc = (vec26 * vec10[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F78);

_04001F7C:	// 4001f7c: <4a0f8e88>	VMACF     vec26 = ( acc += (vec17 * vec15[<none>]) << 1 ) >> 16
	ErrorToCallFunction(0x04001F7C);

_04001F80:	// 4001f80: <ea9c2000>	SQV       [S4 + 0x00] = vec28 <00>
	ErrorToCallFunction(0x04001F80);

_04001F84:	// 4001f84: <22940010>	ADDI      S4 = S4 + 0010
	ErrorToCallFunction(0x04001F84);

_04001F88:	// 4001f88: <1a200003>	BLEZ      (R0<=0) --> 1f98
	sp_reg_pc = 0x04001F88;
	if ((_s32)state.r[S1] <= 0)
	{
		{
			_u32    addr = (0x00000000 + state.r[S2]);
			int     shifter = ((addr & 0xf) << 3);

			if (shifter == 0 && (addr&3) == 0 )
			{
				addr &= 0xfff;
				*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
				*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
				*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
				*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
			}
			else
			{
				_u64    value1 = Load64_DMEM(addr+8);
				_u64    value2 = Load64_DMEM(addr);
				_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
				_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

				if (shifter > 0x40)
				{
					temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
					temp1 = 0;
				} else {
					temp1 = ((temp1 >> shifter) << shifter);
				}
				value1 = value1 & ~temp1;
				value2 = value2 & ~temp2;

				value1 |= (state.v[26].U64[0] & temp1);
				value2 |= (state.v[26].U64[1] & temp2);

				Save64_DMEM(value1, addr+8);
				Save64_DMEM(value2, addr  );
			}
		}
		goto _04001F98;
	}


_04001F8C:	// 4001f8c: <ea5a2000>	SQV       [S2 + 0x00] = vec26 <00>
	{
		_u32    addr = (0x00000000 + state.r[S2]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[26].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[26].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[26].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[26].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[26].U64[0] & temp1);
			value2 |= (state.v[26].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F90:	// 4001f90: <090007b9>	J         04001ee4
	state.r[S2] = state.r[S2] + 0x0010;
	goto _04001EE4;


_04001F94:	// 4001f94: <22520010>	ADDI      S2 = S2 + 0010
	state.r[S2] = state.r[S2] + 0x0010;


_04001F98:	// 4001f98: <eb122002>	SQV       [T8 + 0x20] = vec18 <00>
	{
		_u32    addr = (0x00000020 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[18].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[18].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[18].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[18].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[18].U64[0] & temp1);
			value2 |= (state.v[18].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001F9C:	// 4001f9c: <eb132003>	SQV       [T8 + 0x30] = vec19 <00>
	{
		_u32    addr = (0x00000030 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[19].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[19].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[19].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[19].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[19].U64[0] & temp1);
			value2 |= (state.v[19].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001FA0:	// 4001fa0: <0d000466>	JAL	    04001198
	{
		_u32    addr = (0x00000040 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[24].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[24].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[24].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[24].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[24].U64[0] & temp1);
			value2 |= (state.v[24].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}
	sp_reg_pc = 0x04001FA0;
	state.r[RA] = (sp_reg_pc + 8) & 0x1fff;
	goto _04001198;


_04001FA4:	// 4001fa4: <eb182004>	SQV       [T8 + 0x40] = vec24 <00>
	{
		_u32    addr = (0x00000040 + state.r[T8]);
		int     shifter = ((addr & 0xf) << 3);

		if (shifter == 0 && (addr&3) == 0 )
		{
			addr &= 0xfff;
			*(_u32 *)&pDMEM[addr+12] = state.v[24].U32[0];
			*(_u32 *)&pDMEM[addr+8 ] = state.v[24].U32[1];
			*(_u32 *)&pDMEM[addr+4 ] = state.v[24].U32[2];
			*(_u32 *)&pDMEM[addr   ] = state.v[24].U32[3];
		}
		else
		{
			_u64    value1 = Load64_DMEM(addr+8);
			_u64    value2 = Load64_DMEM(addr);
			_u64    temp1 = 0xFFFFFFFFFFFFFFFF;
			_u64    temp2 = 0xFFFFFFFFFFFFFFFF;

			if (shifter > 0x40)
			{
				temp2 = ((temp2 >> (shifter-0x40)) << (shifter-0x40));
				temp1 = 0;
			} else {
				temp1 = ((temp1 >> shifter) << shifter);
			}
			value1 = value1 & ~temp1;
			value2 = value2 & ~temp2;

			value1 |= (state.v[24].U64[0] & temp1);
			value2 |= (state.v[24].U64[1] & temp2);

			Save64_DMEM(value1, addr+8);
			Save64_DMEM(value2, addr  );
		}
	}


_04001FA8:	// 4001fa8: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001FAC:	// 4001fac: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;


_04001FB0:	// 4001fb0: <c81f2006>	LQV       vec31 <00> = [R0 + 0x60]
	// In function: Tooie1FB0_1FF4
	//Call the function: Tooie1FB0_1FF4
	Tooie1FB0_1FF4();
	goto _04001FF8;

_04001FB4:	// 4001fb4: <3336ffff>	ANDI      S6 = T9 & ffff
	ErrorToCallFunction(0x04001FB4);

_04001FB8:	// 4001fb8: <22d60500>	ADDI      S6 = S6 + 0500
	ErrorToCallFunction(0x04001FB8);

_04001FBC:	// 4001fbc: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001FBC);

_04001FC0:	// 4001fc0: <0019bc02>	SRL       S7 = T9 >> 16
	ErrorToCallFunction(0x04001FC0);

_04001FC4:	// 4001fc4: <22f70500>	ADDI      S7 = S7 + 0500
	ErrorToCallFunction(0x04001FC4);

_04001FC8:	// 4001fc8: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001FC8);

_04001FCC:	// 4001fcc: <489af000>	MTC2      vec30 <00> = K0
	ErrorToCallFunction(0x04001FCC);

_04001FD0:	// 4001fd0: <20150170>	ADDI      S5 = R0 + 0170
	ErrorToCallFunction(0x04001FD0);

_04001FD4:	// 4001fd4: <4bbfe6c0>	VMULF     vec27 = ( acc = (vec28 * vec31[5]) << 1 ) >> 16
	ErrorToCallFunction(0x04001FD4);

_04001FD8:	// 4001fd8: <22b5fff0>	ADDI      S5 = S5 + fff0
	ErrorToCallFunction(0x04001FD8);

_04001FDC:	// 4001fdc: <22f70010>	ADDI      S7 = S7 + 0010
	ErrorToCallFunction(0x04001FDC);

_04001FE0:	// 4001fe0: <22d60010>	ADDI      S6 = S6 + 0010
	ErrorToCallFunction(0x04001FE0);

_04001FE4:	// 4001fe4: <4b1eeec8>	VMACF     vec27 = ( acc += (vec29 * vec30[0]) << 1 ) >> 16
	ErrorToCallFunction(0x04001FE4);

_04001FE8:	// 4001fe8: <cadc2000>	LQV       vec28 <00> = [S6 + 0x00]
	ErrorToCallFunction(0x04001FE8);

_04001FEC:	// 4001fec: <cafd2000>	LQV       vec29 <00> = [S7 + 0x00]
	ErrorToCallFunction(0x04001FEC);

_04001FF0:	// 4001ff0: <1ea0fff8>	BGTZ      (R0>0) --> 1fd4
	ErrorToCallFunction(0x04001FF0);

_04001FF4:	// 4001ff4: <eadb207f>	SQV       [S6 + 0xf0] = vec27 <00>
	ErrorToCallFunction(0x04001FF4);

_04001FF8:	// 4001ff8: <09000443>	J         0400110c
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;
	goto _0400110C;


_04001FFC:	// 4001ffc: <23defff8>	ADDI      S8 = S8 + fff8
	state.r[S8] = state.r[S8] + 0xFFFFFFF8;

	state.halt = 1;
	return;

	_BuildJumpTable:
	#include "jumptable.h"
	goto _Label;
}
