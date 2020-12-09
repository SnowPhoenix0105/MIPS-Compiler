#include "OptimizerFormat.h"

using std::swap;

shared_ptr<IrTable> OptimizerFormat::parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator_ptr)
{
    IrTableBuilder builder;
	IrFactory ir(allocator_ptr);
	IrElemAllocator& allocator = *allocator_ptr;
    for (const auto& code : origin)
    {
        switch (code.head)
        {
		case IrHead::add:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				new_code.elem[2] = code.elem[1];
				new_code.elem[1] = code.elem[2];
			}
			else
			{
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.cst_add(code.elem[2], code.elem[1]);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::sub:	
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			if (!cst_2)
			{
				irelem_t tmp = allocator.alloc_tmp();
				builder.push_back(ir.add(tmp, allocator.zero(), code.elem[1]));
				builder.push_back(ir.sub(code.elem[0], tmp, code.elem[2]));
				break;
			}
			ASSERT(4, IrType::is_imm(code.elem[2]));
			builder.push_back(
				ir.add(
					code.elem[0],
					allocator.zero(),
					allocator.cst_add(
						code.elem[1],
						-allocator.imm_to_value(code.elem[2])))
			);
			break;
		}
		case IrHead::mult:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				new_code.elem[2] = code.elem[1];
				new_code.elem[1] = code.elem[2];
			}
			else
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				ASSERT(4, IrType::is_imm(code.elem[2]));
				new_code.head = IrHead::add;
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.alloc_imm(
					allocator.imm_to_value(code.elem[1])
					* allocator.imm_to_value(code.elem[2])
				);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::div:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				irelem_t tmp = allocator.alloc_tmp();
				builder.push_back(ir.add(tmp, allocator.zero(), code.elem[1]));
				builder.push_back(ir.div(code.elem[0], tmp, code.elem[2]));
				break;
			}
			else
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				ASSERT(4, IrType::is_imm(code.elem[2]));
				new_code.head = IrHead::add;
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.alloc_imm(
					allocator.imm_to_value(code.elem[1])
					/ allocator.imm_to_value(code.elem[2])
				);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::_and:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				new_code.elem[2] = code.elem[1];
				new_code.elem[1] = code.elem[2];
			}
			else
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				ASSERT(4, IrType::is_imm(code.elem[2]));
				new_code.head = IrHead::add;
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.alloc_imm(
					allocator.imm_to_value(code.elem[1])
					& allocator.imm_to_value(code.elem[2])
				);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::_or:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				new_code.elem[2] = code.elem[1];
				new_code.elem[1] = code.elem[2];
			}
			else
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				ASSERT(4, IrType::is_imm(code.elem[2]));
				new_code.head = IrHead::add;
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.alloc_imm(
					allocator.imm_to_value(code.elem[1])
					| allocator.imm_to_value(code.elem[2])
				);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::_nor:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				new_code.elem[2] = code.elem[1];
				new_code.elem[1] = code.elem[2];
			}
			else
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				ASSERT(4, IrType::is_imm(code.elem[2]));
				new_code.head = IrHead::add;
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.alloc_imm(
					~(allocator.imm_to_value(code.elem[1])
					| allocator.imm_to_value(code.elem[2]))
				);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::_xor:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				new_code.elem[2] = code.elem[1];
				new_code.elem[1] = code.elem[2];
			}
			else
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				ASSERT(4, IrType::is_imm(code.elem[2]));
				new_code.head = IrHead::add;
				new_code.elem[1] = allocator.zero();
				new_code.elem[2] = allocator.alloc_imm(
					allocator.imm_to_value(code.elem[1])
					^ allocator.imm_to_value(code.elem[2])
				);
			}
			builder.push_back(new_code);
			break;
		}
		case IrHead::sl:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			if (!cst_2)
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				irelem_t tmp = allocator.alloc_tmp();
				builder.push_back(ir.add(tmp, allocator.zero(), code.elem[1]));
				builder.push_back(ir.sl(code.elem[0], tmp, code.elem[2]));
				break;
			}
			Ir new_code = code;
			ASSERT(4, IrType::is_imm(code.elem[1]));
			ASSERT(4, IrType::is_imm(code.elem[2]));
			new_code.head = IrHead::add;
			new_code.elem[1] = allocator.zero();
			new_code.elem[2] = allocator.alloc_imm(
				allocator.imm_to_value(code.elem[1])
				<< allocator.imm_to_value(code.elem[2])
			);
			builder.push_back(new_code);
			break;
		}
		case IrHead::sr:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				builder.push_back(code);
				break;
			}
			if (!cst_2)
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				irelem_t tmp = allocator.alloc_tmp();
				builder.push_back(ir.add(tmp, allocator.zero(), code.elem[1]));
				builder.push_back(ir.sr(code.elem[0], tmp, code.elem[2]));
				break;
			}
			Ir new_code = code;
			ASSERT(4, IrType::is_imm(code.elem[1]));
			ASSERT(4, IrType::is_imm(code.elem[2]));
			new_code.head = IrHead::add;
			new_code.elem[1] = allocator.zero();
			new_code.elem[2] = allocator.alloc_imm(
				allocator.imm_to_value(code.elem[1])
				>> allocator.imm_to_value(code.elem[2])
			);
			builder.push_back(new_code);
			break;
		}
		case IrHead::less:
		{
			ASSERT(4, !IrType::is_cst(code.elem[0]));
			bool cst_1 = IrType::is_cst(code.elem[1]);
			bool cst_2 = IrType::is_cst(code.elem[2]);
			if (!cst_1)
			{
				if (cst_2)
				{
					int value = allocator.imm_to_value(code.elem[2]);
					if (value > MAX_SIGNED_16 || value < MIN_SIGNED_16)
					{
						irelem_t tmp = allocator.alloc_tmp();
						builder.push_back(ir.add(tmp, allocator.zero(), code.elem[2]));
						builder.push_back(ir.less(code.elem[0], code.elem[1], tmp));
						break;
					}
				}
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (!cst_2)
			{
				ASSERT(4, IrType::is_imm(code.elem[1]));
				irelem_t tmp = allocator.alloc_tmp();
				builder.push_back(ir.add(tmp, allocator.zero(), code.elem[1]));
				builder.push_back(ir.less(code.elem[0], tmp, code.elem[2]));
				break;
			}
			ASSERT(4, IrType::is_imm(code.elem[1]));
			ASSERT(4, IrType::is_imm(code.elem[2]));
			new_code.head = IrHead::add;
			new_code.elem[1] = allocator.zero();
			new_code.elem[2] = allocator.alloc_imm(
				allocator.imm_to_value(code.elem[1])
				< allocator.imm_to_value(code.elem[2])
			);
			builder.push_back(new_code);
			break;
		}
		case IrHead::sw:
		case IrHead::sb:
		{
			if (IrType::is_var(code.elem[0]))
			{
				builder.push_back(code);
				break;
			}
			ASSERT(4, IrType::is_cst(code.elem[0]));
			irelem_t reg = allocator.alloc_tmp();
			builder.push_back(ir.add(reg, allocator.zero(), code.elem[0]));
			Ir new_code = code;
			new_code.elem[0] = reg;
			builder.push_back(new_code);
			break;
		}
		case IrHead::beq:	
		case IrHead::bne:	
		{
			if (code.elem[0] == code.elem[1])
			{
				if (code.head == IrHead::beq)
				{
					builder.push_back(ir._goto(code.elem[2]));
				}
				// bne时恒定不跳转， 故无需将原指令压入
				break;
			}

			bool cst_1 = IrType::is_cst(code.elem[0]);
			bool cst_2 = IrType::is_cst(code.elem[1]);
			if (!cst_1 && !cst_2)
			{
				builder.push_back(code);
				break;
			}
			if (cst_1 && cst_2)
			{
				bool imm_1 = IrType::is_imm(code.elem[0]);
				bool imm_2 = IrType::is_imm(code.elem[1]);
				if (imm_1 && imm_2)
				{
					int v_1 = allocator.imm_to_value(code.elem[0]);
					int v_2 = allocator.imm_to_value(code.elem[1]);
					if (v_1 == v_2 && code.head == IrHead::beq
						|| v_1 != v_2 && code.head == IrHead::bne)
					{
						builder.push_back(ir._goto(code.elem[2]));
					}
					break;
				}
				// 如果两个都是cst且至少有一个不是imm， 则延迟到栈空间分配后再简化
				builder.push_back(code);
				break;
			}
			Ir new_code = code;
			if (cst_1)
			{
				ASSERT(0, !cst_2);
				irelem_t tmp = allocator.alloc_tmp();
				builder.push_back(ir.add(tmp, allocator.zero(), code.elem[0]));
				new_code.elem[0] = tmp;
				builder.push_back(new_code);
				break;
			}
			ASSERT(0, !cst_1 && cst_2);
			irelem_t tmp = allocator.alloc_tmp();
			builder.push_back(ir.add(tmp, allocator.zero(), code.elem[1]));
			new_code.elem[1] = tmp;
			builder.push_back(new_code);
			break;
		}
		case IrHead::protect:
		case IrHead::reload:
			ASSERT(4, false);
        default:
			builder.push_back(code);
            break;
        }
    }


	unordered_set<IrHead> end_set = { IrHead::arr, IrHead::init, IrHead::gvar, IrHead::param, IrHead::func };
	for (int i = 0; i < builder.size(); ++i)
	{
		if (builder[i].head == IrHead::arr ||
			builder[i].head == IrHead::param ||
			builder[i].head == IrHead::init ||
			builder[i].head == IrHead::label
			&& IrType::is_mid(builder[i].elem[0])
			&& IrType::is_func(builder[i].elem[0]))
		{
			for (int j = i - 1; j >= 0; --j)
			{
				if (end_set.count(builder[j].head) != 0) break;
				// if (builder[j].head == IrHead::label && IrType::is_func(builder[i].elem[0]) && IrType::is_beg(builder[i].elem[0])) break;
				swap(builder[j], builder[j + 1]);
			}
		}
	}

	return builder.build();
}
