using VM.Core;

/// <summary>
/// 命名空间规则名称：模块名+Cs，生成的类库与命名空间同名
/// </summary>
namespace RealtimeImageCs
{
    /// <summary>
    /// 界面工具类名称：模块名+Tool
    /// </summary>
    public class RealtimeImageTool : VmModule
    {
        public RealtimeImageTool()
        {
            
        }
        /// <summary>
        /// 构造中要传入模块名称
        /// </summary>
        /// <param name="param"></param>
        public RealtimeImageTool(object param) : base(param, "RealtimeImage")
        {
            
        }
    }
}
