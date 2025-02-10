using ABI.CCK.Components;
using ABI_RC.API;
using ABI_RC.Core.InteractionSystem;
using ABI_RC.Core.InteractionSystem.Base;
using ABI_RC.Core.IO;
using ABI_RC.Core.Networking;
using ABI_RC.Core.Networking.API.Responses;
using ABI_RC.Core.Networking.IO.UserGeneratedContent;
using ABI_RC.Core.Player;
using ABI_RC.Core.Savior;
using ABI_RC.Core.Util;
using ABI_RC.Systems.Camera.VisualMods;
using ABI_RC.Systems.Gravity;
using ABI_RC.Systems.Movement;
using ABI_RC.VideoPlayer.Scripts;
using ABI_RC.VideoPlayer.Scripts.Players.AvPro;
using Bhaptics.Tact.Unity;
using DarkRift;
using MelonLoader;
using Michsky.UI.ModernUIPack;
using RTG;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Runtime.Remoting.Messaging;
using System.Security;
using System.Security.Permissions;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.Rendering;



namespace stupidGamemod
{
    public class modClass : MelonMod
    {

        private short massMode = 0;
        private bool propProtection = false;
        private bool attachPlayer = false;
        CVRPlayerEntity playerAttached = null;


        static string ReplaceWithRandom(string input, System.Random random)
        {
            string pattern = @"~([A-Z0-9]+)$";

            
                input = Regex.Replace(input, pattern, match =>
                {
                    string original = match.Groups[1].Value;
                    string randomString = GenerateRandomString(original.Length, random);
                    return $"~{randomString}";
                });
            

            return input;
        }

        private IEnumerator WaitAndExecute(float delay)
        {

            yield return new WaitForSeconds(delay); 

        }

        static void updateObjectData(CVRPlayerEntity playerAttached)
        {
            float propDisplace = .2f;
            foreach (CVRSyncHelper.PropData prop in CVRSyncHelper.Props)
            {

                using (DarkRiftWriter darkRiftWriter = DarkRiftWriter.Create())
                {
                    darkRiftWriter.Write(prop.InstanceId);
                    darkRiftWriter.Write(playerAttached.PlayerObject.transform.position.x);
                    darkRiftWriter.Write(playerAttached.PlayerObject.transform.position.y + propDisplace);
                    darkRiftWriter.Write(playerAttached.PlayerObject.transform.position.z);

                    darkRiftWriter.Write(playerAttached.PlayerObject.transform.rotation.x);
                    darkRiftWriter.Write(playerAttached.PlayerObject.transform.rotation.y);
                    darkRiftWriter.Write(playerAttached.PlayerObject.transform.rotation.z);

                    darkRiftWriter.Write(3f);
                    darkRiftWriter.Write(3f);
                    darkRiftWriter.Write(3f);

                    darkRiftWriter.Write(prop.CustomFloatsAmount);
                    for (int i = 0; i < prop.CustomFloatsAmount; i++)
                    {
                        darkRiftWriter.Write(prop.CustomFloats[i]);
                    }

                    darkRiftWriter.Write("SYSTEM");
                    darkRiftWriter.Write("SYSTEM");
                    darkRiftWriter.Write(0);


                    using (Message message = Message.Create(10051, darkRiftWriter))
                    {
                        CVRSyncHelper.updatePropFromNetwork(message);
                    }




                }
                propDisplace -= 0.1f;
            }
        }
        static string GenerateRandomString(int length, System.Random random)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            StringBuilder builder = new StringBuilder(length);

            for (int i = 0; i < length; i++)
            {
                builder.Append(chars[random.Next(chars.Length)]);
            }

            return builder.ToString();
        }

        private void gamePrint(string msgTop, string msgBottom)
        {
            ViewManager managers = UnityEngine.GameObject.FindObjectOfType<ViewManager>();
            managers.NotifyUser(msgTop, msgBottom, 3f);
        }

        List<string> ids = new List<string>();

        public override void OnInitializeMelon()
        {
            LoggerInstance.Msg("stupid menu");
        }
        public override void OnUpdate()
        {
            if (Input.GetKeyDown(KeyCode.F1)) 
            {
                CVRWorld world = UnityEngine.GameObject.FindObjectOfType<CVRWorld>();
                GravitySystem.WorldPlayerGravity = world.gravity * -Math.Sign(GravitySystem.WorldPlayerGravity);
                if (GravitySystem.WorldPlayerGravity < 0)
                {
                    gamePrint("reverse", "player gravity");
                }
                else
                {
                    gamePrint("normal", "player gravity");
                }
                
                

            }
            if (Input.GetKeyDown(KeyCode.F2))
            {
                CVRWorld world = UnityEngine.GameObject.FindObjectOfType<CVRWorld>();
                GravitySystem.WorldObjectGravity = world.objectGravity = -Math.Sign(GravitySystem.WorldObjectGravity);
                if (GravitySystem.WorldObjectGravity < 0)
                {
                    gamePrint("reverse", "object gravity");
                }
                else
                {
                    gamePrint("normal", "object gravity");
                }

            }
            if (Input.GetKeyDown(KeyCode.F3))
            {
                
                if (propProtection)
                {
                    ids.Clear();
                    propProtection = false;
                    LoggerInstance.Msg("prop Protection OFF");
                    gamePrint("propProtection", "Off");
                    LoggerInstance.Msg("list start");
                    foreach (string id in ids)
                    {
                        LoggerInstance.Msg(id);
                    }
                    LoggerInstance.Msg("list end");
                }
                else
                {
                    CVRPlayerManager t = UnityEngine.GameObject.FindObjectOfType<CVRPlayerManager>();
                    foreach (CVRPlayerEntity p in t.NetworkPlayers)
                    {
                        ids.Add(p.Uuid);
                    }
                    propProtection = true;
                    LoggerInstance.Msg("prop Protection ON");
                    gamePrint("propProtection", "On");
                    LoggerInstance.Msg("list start");
                    foreach(string id in ids)
                    {
                        LoggerInstance.Msg(id);
                    }
                    LoggerInstance.Msg("list end");
                }


            }
            if (Input.GetKeyDown(KeyCode.F4))
            {
                BetterBetterCharacterController controller = UnityEngine.GameObject.FindObjectOfType<BetterBetterCharacterController>();
                switch (massMode)
                {
                    case 0:
                        massMode += 1;
                        controller.mass = 20;
                        gamePrint("20", "mass change");
                        break;
                    case 1:
                        massMode += 1;
                        controller.mass = 1000;
                        gamePrint("1000", "mass change");
                        break;
                    case 2:
                        massMode = 0;
                        controller.mass = float.MaxValue;
                        gamePrint(float.MaxValue.ToString(), "mass change");
                        break;
                }

            }
            if (Input.GetKeyDown(KeyCode.F5))
            {

                PlayerSetup player = UnityEngine.GameObject.FindObjectOfType<PlayerSetup>();
                LoggerInstance.Msg($"object: {player.propGuidForSpawn}");
                float propDisplace = .1f;
                foreach (CVRSyncHelper.PropData prop in CVRSyncHelper.Props)
                {

                    using (DarkRiftWriter darkRiftWriter = DarkRiftWriter.Create())
                    {
                        darkRiftWriter.Write(prop.InstanceId);
                        darkRiftWriter.Write(player.transform.position.x);
                        darkRiftWriter.Write(player.transform.position.y + propDisplace);
                        darkRiftWriter.Write(player.transform.position.z);

                        darkRiftWriter.Write(player.transform.rotation.x);
                        darkRiftWriter.Write(player.transform.rotation.y);
                        darkRiftWriter.Write(player.transform.rotation.z);

                        darkRiftWriter.Write(3f);
                        darkRiftWriter.Write(3f);
                        darkRiftWriter.Write(3f);

                        darkRiftWriter.Write(prop.CustomFloatsAmount);
                        for (int i = 0; i < prop.CustomFloatsAmount; i++)
                        {
                            darkRiftWriter.Write(prop.CustomFloats[i]);
                        }

                        darkRiftWriter.Write("SYSTEM");
                        darkRiftWriter.Write("SYSTEM");
                        darkRiftWriter.Write(0);


                        using (Message message = Message.Create(10051, darkRiftWriter))
                        {
                            CVRSyncHelper.updatePropFromNetwork(message);
                        }




                    }
                    propDisplace += 0.1f;
                }


            }
            if (Input.GetKeyDown(KeyCode.F6))
            {
                BetterBetterCharacterController controller = UnityEngine.GameObject.FindObjectOfType<BetterBetterCharacterController>();
                
                foreach (CVRSyncHelper.PropData prop in CVRSyncHelper.Props)
                {
                    CVRSyncHelper.SpawnProp(prop.ObjectId, controller.position.x, controller.position.y, controller.position.z, true);
                }
                controller.Jump();
            }
            if (Input.GetKeyDown(KeyCode.F9))
            {
                updateObjectData(playerAttached);

            }
            if (Input.GetKeyDown(KeyCode.Keypad4))
            {
                CVRPlayerManager manager = UnityEngine.GameObject.FindObjectOfType<CVRPlayerManager>();
               
            }
            

            if (Input.GetKeyDown(KeyCode.F8))
            {
                
                    CVRPlayerManager manager = UnityEngine.GameObject.FindObjectOfType<CVRPlayerManager>();
                    for (int i = 0; i < manager.NetworkPlayers.Count; i++)
                    {
                        LoggerInstance.Msg($"{i}) {manager.NetworkPlayers[i].Username}");
                    }
                    Console.Write("select player: ");
                    string selectPlayer = Console.ReadLine();
                    int selectPlayerInt = Convert.ToInt32(selectPlayer);
                    playerAttached = manager.NetworkPlayers[selectPlayerInt];
                    gamePrint("On", $"attached to {manager.NetworkPlayers[selectPlayerInt].Username}");


            }
            if (Input.GetKeyDown(KeyCode.Keypad1))
            {

                foreach (Pickupable prop in UnityEngine.GameObject.FindObjectsOfType<Pickupable>())
                {
                    LoggerInstance.Msg("pickup found");
                    typeof(Pickupable).GetField("<IsPickupable>k__BackingField", BindingFlags.Public | BindingFlags.Instance | BindingFlags.NonPublic).SetValue(prop, false);
                    LoggerInstance.Msg($"new prop is Pickuable: {prop.IsPickupable}");
                }

            }
            if (Input.GetKeyDown(KeyCode.Keypad2))
            {

                PlayerDescriptor[] playerDescript = UnityEngine.GameObject.FindObjectsOfType<PlayerDescriptor>();
                foreach (PlayerDescriptor playerDescriptor in playerDescript)
                {
                    playerDescriptor.userName = "im so dumb";
                }


                
            }

            if (Input.GetKeyDown(KeyCode.Keypad3))
            {
                foreach (CVRPortalManager manager in UnityEngine.GameObject.FindObjectsOfType<CVRPortalManager>())
                {
                    LoggerInstance.Msg("inf portal");
                    manager.portalTime = 10000000;
                }


            }


            MetaPort metaPort = UnityEngine.GameObject.FindObjectOfType<MetaPort>();
            if (!metaPort.worldAllowProps)
            {
                metaPort.worldAllowProps = true;
                metaPort.SaveGameConfig();
                LoggerInstance.Msg("changed allow props");
                gamePrint("worldAllowProps", "set to true");

            }                                                                                                                                                              


            if (attachPlayer && playerAttached != null)
            {
                updateObjectData(playerAttached);
                WaitAndExecute(5);
                
            }

            if (propProtection)
            {
                CVRSpawnable[] propsLOL = UnityEngine.GameObject.FindObjectsOfType<CVRSpawnable>();
                foreach (CVRSpawnable p in propsLOL)
                {
                    
                    if (ids.Contains(p.ownerId))
                    {
                        CVRSyncHelper.DeleteMyPropByInstanceIdOverNetwork(p.GetInstanceID().ToString());
                        UnityEngine.GameObject.Destroy(p.gameObject);
                    }
                }
                
            }
        }
    }
}
