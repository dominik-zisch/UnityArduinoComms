using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

// Originally written by keijiro for his OscJack Unity asset.
// Adapted for use with this Arduino asset.
// Original code:
// https://github.com/keijiro/OscJack

[CanEditMultipleObjects, CustomEditor(typeof(ArduinoEventReceiver))]
public class ArduinoEventReceiverEditor : Editor
{
    SerializedProperty _arduino;
    SerializedProperty _command;
    SerializedProperty _dataType;
    
    SerializedProperty _bytesReceived;
    SerializedProperty _boolReceived;
    SerializedProperty _intReceived;
    SerializedProperty _floatReceived;
    SerializedProperty _stringReceived;
    SerializedProperty _int2Received;
    SerializedProperty _int3Received;
    SerializedProperty _vector2Received;
    SerializedProperty _vector3Received;
    SerializedProperty _vector4Received;

    static class Labels
    {
        public static readonly GUIContent Arduino = new GUIContent("Arduino Handler");
        public static readonly GUIContent Command = new GUIContent("Commmand Byte");
    }

    void OnEnable()
    {
        _arduino = serializedObject.FindProperty("_arduino");
        _command = serializedObject.FindProperty("_command");
        _dataType = serializedObject.FindProperty("_dataType");
        
        _bytesReceived = serializedObject.FindProperty("_bytesReceived");
        _boolReceived = serializedObject.FindProperty("_boolReceived");
        _intReceived = serializedObject.FindProperty("_intReceived");
        _floatReceived = serializedObject.FindProperty("_floatReceived");
        _stringReceived = serializedObject.FindProperty("_stringReceived");
        _int2Received = serializedObject.FindProperty("_int2Received");
        _int3Received = serializedObject.FindProperty("_int3Received");
        _vector2Received = serializedObject.FindProperty("_vector2Received");
        _vector3Received = serializedObject.FindProperty("_vector3Received");
        _vector4Received = serializedObject.FindProperty("_vector4Received");
    }
    
    public override void OnInspectorGUI()
    {
        serializedObject.Update();

        EditorGUILayout.ObjectField(_arduino, Labels.Arduino);
        EditorGUILayout.DelayedIntField(_command, Labels.Command);
        EditorGUILayout.PropertyField(_dataType);

        if (!_dataType.hasMultipleDifferentValues)
        {
            switch ((ArduinoDataType)_dataType.enumValueIndex+1)
            {
                case ArduinoDataType.Bytearray:
                    EditorGUILayout.PropertyField(_bytesReceived);
                    break;
                case ArduinoDataType.Bool:
                    EditorGUILayout.PropertyField(_boolReceived);
                    break;
                case ArduinoDataType.Int:
                    EditorGUILayout.PropertyField(_intReceived);
                    break;
                case ArduinoDataType.Float:
                    EditorGUILayout.PropertyField(_floatReceived);
                    break;
                case ArduinoDataType.String:
                    EditorGUILayout.PropertyField(_stringReceived);
                    break;
                case ArduinoDataType.Int2:
                    EditorGUILayout.PropertyField(_int2Received);
                    break;
                case ArduinoDataType.Int3:
                    EditorGUILayout.PropertyField(_int3Received);
                    break;
                case ArduinoDataType.Vector2:
                    EditorGUILayout.PropertyField(_vector2Received);
                    break;
                case ArduinoDataType.Vector3:
                    EditorGUILayout.PropertyField(_vector3Received);
                    break;
                case ArduinoDataType.Vector4:
                    EditorGUILayout.PropertyField(_vector4Received);
                    break;
            }
        }

        serializedObject.ApplyModifiedProperties();
    }
}
