//Using these to read usages/temps
using LibreHardwareMonitor.Hardware;
using System;
using System.Threading;

//IVisitor updates all hardware components
public class UpdateVisitor : IVisitor
{
    public void VisitComputer(IComputer computer) => computer.Traverse(this);
    //Updates each hardware device and sub devices, finding all sensors
    public void VisitHardware(IHardware hardware)
    {
        hardware.Update();
        foreach (IHardware subHardware in hardware.SubHardware)
            subHardware.Accept(this);
    }
    public void VisitSensor(ISensor sensor) { }
    public void VisitParameter(IParameter parameter) { }
}

public class Program
{
    public static void Main()
    {
        //Enabling monitoring, opens system and sleep to stabilize system
        var computer = new Computer
        {
            IsCpuEnabled = true,
            IsGpuEnabled = true,
            IsMemoryEnabled = true,
            IsStorageEnabled = true
        };
        computer.Open();
        Thread.Sleep(300);
        var visitor = new UpdateVisitor();

        while (true){
            //Updating and sleeping for all hardware readings
            computer.Accept(visitor);
            Thread.Sleep(150);

            //Variables for getting the readings
            float? cpuTemp = null;
            float? cpuUsage = null;
            float? gpuTemp = null;
            float maxGpuUsage = -1f;
            float? ramUsage = null;
            float maxDiskUsage = -1f;

            //Going through every device individually
            foreach (IHardware hardware in computer.Hardware)
                {
                    hardware.Update();
                    Thread.Sleep(50);
                    //Going through every sensor individually
                    foreach (ISensor sensor in hardware.Sensors)
                    {
                        //Forget it if it's null
                        if (sensor.Value == null || float.IsNaN(sensor.Value.Value)) continue;
                        string sensorName = sensor.Name.ToLower();

                        //Switch for what type it is
                        switch (hardware.HardwareType)
                        {   
                            //If it's CPU, execute this block to determine if it's usage or temp
                            case HardwareType.Cpu:
                                //Usage block, write to cpuUsage if that's the case
                                if (sensor.SensorType == SensorType.Load && sensor.Name == "CPU Total")
                                    cpuUsage = (float)Math.Round(sensor.Value.Value, 1);

                                //Temp block, write to cpuTemp if that's the case
                                if (sensor.SensorType == SensorType.Temperature &&
                                    (sensor.Name.Contains("CPU") || sensor.Name.Contains("Tctl/Tdie")))
                                    cpuTemp = sensor.Value.Value;
                                break;
                            //RAM block, I think it's just memory usage that can be read from the PC
                            case HardwareType.Memory:
                                if (sensor.SensorType == SensorType.Load && sensor.Name == "Memory")
                                    ramUsage = sensor.Value.Value;
                                break;

                            //This was a bit annoying, different manufacturers have different names, not all just GPU
                            case HardwareType.GpuAmd:
                            case HardwareType.GpuIntel:
                            case HardwareType.GpuNvidia:
                                //This was also a bit annoying, there's so many temp readings off GPU's, I believe this is the hottest
                                if (sensor.SensorType == SensorType.Temperature &&
                                    sensorName.Contains("hot spot") &&
                                    gpuTemp == null)
                                    gpuTemp = sensor.Value.Value;
                                
                                //3d is what LibreHardwareMonitor put out, not really sure why but I'm gonna read it!
                                if (sensor.SensorType == SensorType.Load && sensorName.Contains("3d"))
                                    if (sensor.Value.Value > maxGpuUsage)
                                        maxGpuUsage = sensor.Value.Value;
                                break;

                            //Disk usage, pretty much like the rest
                            case HardwareType.Storage:
                                if (sensor.SensorType == SensorType.Load &&
                                    (sensorName.Contains("usage") || sensorName.Contains("activity") || sensorName.Contains("total")))
                                    if (sensor.Value.Value > maxDiskUsage)
                                        maxDiskUsage = sensor.Value.Value;
                                break;
                        }
                    }
                }

            // Printing all of these values to the console for later reading!
            // Don't print if there's no value, happened a lot actually
            if (cpuUsage.HasValue)
                Console.WriteLine($"CPU_USAGE={cpuUsage.Value:F1}");
            if (cpuTemp.HasValue)
                Console.WriteLine($"CPU_TEMP={cpuTemp.Value:F1}");
            if (gpuTemp.HasValue)
                Console.WriteLine($"GPU_TEMP={gpuTemp.Value:F1}");
            if (maxGpuUsage >= 0)
                Console.WriteLine($"GPU_USAGE={maxGpuUsage:F1}");
            if (ramUsage.HasValue)
                Console.WriteLine($"RAM_USAGE={ramUsage.Value:F1}");
            if (maxDiskUsage >= 0)
                Console.WriteLine($"DISK_USAGE={maxDiskUsage:F1}");
            //For debug purposes
            Console.WriteLine("END_FRAME");
            //If you don't sleep or pause, the system won't read correctly weirdly enough
            Thread.Sleep(1000);
        }

    }
}
