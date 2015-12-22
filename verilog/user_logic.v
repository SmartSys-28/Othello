//----------------------------------------------------------------------------
// user_logic.vhd - module
//----------------------------------------------------------------------------
//
// ***************************************************************************
// ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
// **                                                                       **
// ** Xilinx, Inc.                                                          **
// ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
// ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
// ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
// ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
// ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
// ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
// ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
// ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
// ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
// ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
// ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
// ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
// ** FOR A PARTICULAR PURPOSE.                                             **
// **                                                                       **
// ***************************************************************************
//
//----------------------------------------------------------------------------
// Filename:          user_logic.vhd
// Version:           1.00.a
// Description:       User logic module.
// Date:              Sat Dec 01 11:20:05 2012 (by Create and Import Peripheral Wizard)
// Verilog Standard:  Verilog-2001
//----------------------------------------------------------------------------
// Naming Conventions:
//   active low signals:                    "*_n"
//   clock signals:                         "clk", "clk_div#", "clk_#x"
//   reset signals:                         "rst", "rst_n"
//   generics:                              "C_*"
//   user defined types:                    "*_TYPE"
//   state machine next state:              "*_ns"
//   state machine current state:           "*_cs"
//   combinatorial signals:                 "*_com"
//   pipelined or register delay signals:   "*_d#"
//   counter signals:                       "*cnt*"
//   clock enable signals:                  "*_ce"
//   internal version of output port:       "*_i"
//   device pins:                           "*_pin"
//   ports:                                 "- Names begin with Uppercase"
//   processes:                             "*_PROCESS"
//   component instantiations:              "<ENTITY_>I_<#|FUNC>"
//----------------------------------------------------------------------------

module user_logic
(
  // -- ADD USER PORTS BELOW THIS LINE ---------------
  // --USER ports added here 
  CLK_25M,
  LED,
  VGA_R,
  VGA_G,
  VGA_B,
  VGA_HSYNC,
  VGA_VSYNC,
  // -- ADD USER PORTS ABOVE THIS LINE ---------------

  // -- DO NOT EDIT BELOW THIS LINE ------------------
  // -- Bus protocol ports, do not add to or delete 
  Bus2IP_Clk,                     // Bus to IP clock
  Bus2IP_Reset,                   // Bus to IP reset
  Bus2IP_Data,                    // Bus to IP data bus
  Bus2IP_BE,                      // Bus to IP byte enables
  Bus2IP_RdCE,                    // Bus to IP read chip enable
  Bus2IP_WrCE,                    // Bus to IP write chip enable
  IP2Bus_Data,                    // IP to Bus data bus
  IP2Bus_RdAck,                   // IP to Bus read transfer acknowledgement
  IP2Bus_WrAck,                   // IP to Bus write transfer acknowledgement
  IP2Bus_Error                    // IP to Bus error response
  // -- DO NOT EDIT ABOVE THIS LINE ------------------
); // user_logic

// -- ADD USER PARAMETERS BELOW THIS LINE ------------
// --USER parameters added here 
parameter h_a = 96, h_b = 48, h_c = 640, h_d = 16, h_total= h_a + h_b + h_c + h_d;
parameter v_a = 2, v_b = 33, v_c = 480, v_d = 10, v_total = v_a + v_b + v_c + v_d;
parameter chesscolor1 = 8'b11111111;
parameter chesscolor2 = 8'b00000000;
parameter boardcolor  = 8'b11101100;
parameter cursorcolor = 8'b11100000;
parameter linecolor   = 8'b00000000;
parameter menulinecolor = 8'b11100000;
parameter menucolor = 8'b10010010;
// -- ADD USER PARAMETERS ABOVE THIS LINE ------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol parameters, do not add to or delete
parameter C_SLV_DWIDTH                   = 32;
parameter C_NUM_REG                      = 1;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

// -- ADD USER PORTS BELOW THIS LINE -----------------
// --USER ports added here 
input                                     CLK_25M;
output     [0 : 7]                        LED;
output     [0 : 2]                        VGA_R;
output     [0 : 2]                        VGA_G;
output     [0 : 1]                        VGA_B;
output                                    VGA_HSYNC;
output                                    VGA_VSYNC;
// -- ADD USER PORTS ABOVE THIS LINE -----------------

// -- DO NOT EDIT BELOW THIS LINE --------------------
// -- Bus protocol ports, do not add to or delete
input                                     Bus2IP_Clk;
input                                     Bus2IP_Reset;
input      [0 : C_SLV_DWIDTH-1]           Bus2IP_Data;
input      [0 : C_SLV_DWIDTH/8-1]         Bus2IP_BE;
input      [0 : C_NUM_REG-1]              Bus2IP_RdCE;
input      [0 : C_NUM_REG-1]              Bus2IP_WrCE;
output     [0 : C_SLV_DWIDTH-1]           IP2Bus_Data;
output                                    IP2Bus_RdAck;
output                                    IP2Bus_WrAck;
output                                    IP2Bus_Error;
// -- DO NOT EDIT ABOVE THIS LINE --------------------

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // --USER nets declarations added here, as needed for user logic 
  reg        [0 : 9]                        h_cnt;
  reg        [0 : 9]                        v_cnt;
  reg        [0 : 0]                        h_sync;
  reg        [0 : 0]                        v_sync;
  reg        [0 : 0]                        vga_ena;
  reg        [0 : 7]                        vga_data;
  reg        [0 : 9]                        x_cor;
  reg        [0 : 9]                        y_cor;
  reg        [0 : 2]                        board[0 : 63];
  reg        [0 : 1]                        menu[0 : 299];
  reg        [0 : 7]                        colormap[0 : 3];
  reg        [0 : 9]                        x_center;
  reg        [0 : 9]                        y_center;
  reg        [0 : 5]                        x_pos;
  reg        [0 : 5]                        y_pos;
  reg        [0 : 7]                        board_pos;
  reg        [0 : 9]                        menu_pos;
  integer                                   i;
  // Nets for user logic slave model s/w accessible register example
  reg        [0 : C_SLV_DWIDTH-1]           slv_reg0;
  wire       [0 : 0]                        slv_reg_write_sel;
  wire       [0 : 0]                        slv_reg_read_sel;
  reg        [0 : C_SLV_DWIDTH-1]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;
  integer                                   byte_index, bit_index;

  // --USER logic implementation added here
  initial
  begin
    colormap[0] = chesscolor1;
	 colormap[1] = chesscolor2;
	 colormap[2] = boardcolor;
	 colormap[3] = cursorcolor;
  end
  
  always @( posedge Bus2IP_Clk )
  begin
    if ( Bus2IP_Reset == 1 )
	   begin
		  for (i = 0; i < 64; i=i+1)
		    board[i] <= 3'b010;
		  for (i = 0; i < 300; i=i+1)
		    menu[i] <= 2'b10;
		end
	 else
	   begin
        if ( Bus2IP_WrCE == 1'b1)
		    begin
			   if (Bus2IP_Data[16 : 23]== 0)
		        board[Bus2IP_Data[0 : 7] + Bus2IP_Data[8 : 15] * 8] <= Bus2IP_Data[24 : 31];
				else if (Bus2IP_Data[16 : 23]== 1)
				  menu[Bus2IP_Data[0 : 7] + Bus2IP_Data[8 : 15] * 10] <= Bus2IP_Data[24 : 31];
			 end
		end
  end
  
  always @( posedge CLK_25M )
  begin
    if (Bus2IP_Reset == 1)
	   begin
		  h_cnt <= 0;
		  v_cnt <= 0;
		end
	 else
	   begin
        if (v_cnt == v_total-1)
			 v_cnt <= 0;
		  if (h_cnt == h_total-1)
			 begin 
		      h_cnt <= 0;
				v_cnt <= v_cnt+1;
			 end
		  else
			 h_cnt <= h_cnt+1;
      end
  end
  
  always@(*)
  begin
    if (h_cnt < h_a)
	   h_sync <= 0;
	 else		     
		h_sync <= 1;
	 if (v_cnt < v_a)
		v_sync <= 0;
	 else
		v_sync <= 1;	
    if ((h_cnt >= h_a + h_b && h_cnt < h_a + h_b + h_c) && (v_cnt >= v_a + v_b && v_cnt < v_a + v_b + v_c))
	   begin
		  vga_ena <= 1;
		  x_cor <= h_cnt - h_a - h_b;
		  y_cor <= v_cnt - v_a - v_b;
		end
    else 
		vga_ena <= 0;	  
  end
  // ----------------------棋盘绘制和旗子绘制部分：开始-----------------------------------------------------------
  always@(*)
  begin
    if (vga_ena == 1 && x_cor < 480 && y_cor < 480)
	 // 开始绘制屏幕左边480*480的board部分==========================================================
	   begin
		  if ((x_cor >= 40 && x_cor <= 440 && y_cor >= 40 && y_cor <= 440))
	       begin
		    if (x_cor == 40 || x_cor == 90 || x_cor == 140 || x_cor == 190 || x_cor == 240 || x_cor == 290 || x_cor == 340 || x_cor == 390 || x_cor == 440
			  || y_cor == 40 || y_cor == 90 || y_cor == 140 || y_cor == 190 || y_cor == 240 || y_cor == 290 || y_cor == 340 || y_cor == 390 || y_cor == 440)
		      vga_data <= linecolor;                      // 第一个if，来绘制出棋盘的线
		    else
		      begin
			     x_pos <= (x_cor - 40) / 50;                // 旗子在棋盘上的x轴位置
				  y_pos <= (y_cor - 40) / 50;                // 旗子在棋盘上的Y轴位置
				  board_pos <= x_pos + y_pos * 8;            // 旗子位置在board数组上的位置
				  x_center <= x_pos * 50 + 40 + 25;    // 绘制特定旗子的圆心x坐标
				  y_center <= y_pos * 50 + 40 + 25;    // 绘制特定旗子的圆心y坐标
				  if (board[board_pos] == 3'b100 ) // 第三个if，来判断现在是否是绘制白色小旗子（位置判断）
					 begin 
						if ((x_cor - x_center) * (x_cor - x_center) + (y_cor - y_center) * (y_cor - y_center) <= 36) // 给白色小旗子绘制圆
						  vga_data <= chesscolor1;
						else
						  vga_data <= boardcolor;
				    end
				  else if (board[board_pos] == 3'b101 ) // 第三个if，来判断现在是否是绘制黑色小旗子（位置判断）
					 begin 
						if ((x_cor - x_center) * (x_cor - x_center) + (y_cor - y_center) * (y_cor - y_center) <= 36) // 给黑色小旗子绘制圆
						  vga_data <= chesscolor2;
						else
						  vga_data <= boardcolor;
				    end
				  else if ((x_cor - x_center) * (x_cor - x_center) + (y_cor - y_center) * (y_cor - y_center) <= 400) // 给大旗子绘制圆
					 vga_data <= colormap[board[board_pos]]; 
				  else
				    vga_data <= boardcolor;
		      end
		    end
		  else
		    vga_data <= boardcolor;
		end
		// board部分的绘制结束=========================================================================
	 else if (vga_ena == 1 && x_cor >= 480 && x_cor <= 640 && y_cor < 480)
	 // 开始绘制右边160*480的menu部分==================================================================
	   begin
		  menu_pos <= (x_cor - 480) / 16 + y_cor / 16 * 10;
		  if (menu_pos < 300)
		    begin
		      if (menu[menu_pos] == 2'b10)
              vga_data <= menucolor;
            else
              vga_data <= colormap[menu[menu_pos]];
		    end
		  else
		    vga_data <= 0;
		end
		// menu部分的绘制结束==========================================================================
	 else
	   vga_data <= 0;
  end
  
  // ----------------------棋盘绘制和旗子绘制部分：结束-----------------------------------------------------------
  
  assign LED = 0;
  assign VGA_R = vga_data[0 : 2];
  assign VGA_G = vga_data[3 : 5];
  assign VGA_B = vga_data[6 : 7];
  assign VGA_HSYNC = h_sync;
  assign VGA_VSYNC = v_sync;

  // ------------------------------------------------------
  // Example code to read/write user logic slave model s/w accessible registers
  // 
  // Note:
  // The example code presented here is to show you one way of reading/writing
  // software accessible registers implemented in the user logic slave model.
  // Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  // to one software accessible register by the top level template. For example,
  // if you have four 32 bit software accessible registers in the user logic,
  // you are basically operating on the following memory mapped registers:
  // 
  //    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  //                     "1000"   C_BASEADDR + 0x0
  //                     "0100"   C_BASEADDR + 0x4
  //                     "0010"   C_BASEADDR + 0x8
  //                     "0001"   C_BASEADDR + 0xC
  // 
  // ------------------------------------------------------

  assign
    slv_reg_write_sel = Bus2IP_WrCE[0:0],
    slv_reg_read_sel  = Bus2IP_RdCE[0:0],
    slv_write_ack     = Bus2IP_WrCE[0],
    slv_read_ack      = Bus2IP_RdCE[0];

  // implement slave model register(s)
  always @( posedge Bus2IP_Clk )
    begin: SLAVE_REG_WRITE_PROC

      if ( Bus2IP_Reset == 1 )
        begin
          slv_reg0 <= 0;
        end
      else
        case ( slv_reg_write_sel )
          1'b1 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                  slv_reg0[bit_index] <= Bus2IP_Data[bit_index];
          default : ;
        endcase

    end // SLAVE_REG_WRITE_PROC

  // implement slave model register read mux
  always @( slv_reg_read_sel or slv_reg0 )
    begin: SLAVE_REG_READ_PROC

      case ( slv_reg_read_sel )
        1'b1 : slv_ip2bus_data <= slv_reg0;
        default : slv_ip2bus_data <= 0;
      endcase

    end // SLAVE_REG_READ_PROC

  // ------------------------------------------------------------
  // Example code to drive IP to Bus signals
  // ------------------------------------------------------------

  assign IP2Bus_Data    = slv_ip2bus_data;
  assign IP2Bus_WrAck   = slv_write_ack;
  assign IP2Bus_RdAck   = slv_read_ack;
  assign IP2Bus_Error   = 0;

endmodule
