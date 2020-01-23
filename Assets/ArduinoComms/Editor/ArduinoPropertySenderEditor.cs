using UnityEngine;
using UnityEditor;
using System;
using System.Reflection;
using System.Collections.Generic;
using System.Linq;

// Originally written by keijiro for his OscJack Unity asset.
// Adapted for use with this Arduino asset.
// Original code:
// https://github.com/keijiro/OscJack

[CanEditMultipleObjects, CustomEditor(typeof(ArduinoPropertySender))]
public class ArduinoPropertySenderEditor : Editor
{
    
    SerializedProperty _arduino;
    SerializedProperty _framerate;
    SerializedProperty _command;
    SerializedProperty _dataSource;
    SerializedProperty _propertyName;
    
    static class Labels
    {
        public static readonly GUIContent Arduino = new GUIContent("Arduino Handler");
        public static readonly GUIContent Framerate = new GUIContent("Framerate");
        public static readonly GUIContent Command = new GUIContent("Command");
    }
    
    void OnEnable()
    {
        _arduino = serializedObject.FindProperty("_arduino");
        _framerate = serializedObject.FindProperty("_framerate");
        _command = serializedObject.FindProperty("_command");
        _dataSource = serializedObject.FindProperty("_dataSource");
        _propertyName = serializedObject.FindProperty("_propertyName");
    }
    
    public override void OnInspectorGUI()
    {
        serializedObject.Update();

        EditorGUILayout.ObjectField(_arduino, Labels.Arduino);
        EditorGUILayout.DelayedIntField(_framerate, Labels.Framerate);
        EditorGUILayout.DelayedIntField(_command, Labels.Command);
        EditorGUILayout.PropertyField(_dataSource);

        if (!_dataSource.hasMultipleDifferentValues &&
            !_propertyName.hasMultipleDifferentValues)
        {
            // Custom drop-down UI
            ShowComponentSelector();
            ShowPropertySelector();
        }
        else
        {
            // Normal text field
            EditorGUILayout.PropertyField(_propertyName);
        }

        serializedObject.ApplyModifiedProperties();
    }
    
    void ShowComponentSelector()
    {
        var component = (Component)_dataSource.objectReferenceValue;
        if (component == null) return;

        // Cache the component list in the parent game object.
        CacheComponentList(component.gameObject);

        // Show the component selection drop-down list.
        var index = Array.IndexOf(_componentList, component.GetType().Name);
        var newIndex = EditorGUILayout.Popup("Component", index, _componentList);

        // Update the component if the selection was changed.
        if (index != newIndex)
        {
            component = component.GetComponent(_componentList[newIndex]);
            _dataSource.objectReferenceValue = component;
        }
    }

    void ShowPropertySelector()
    {
        var component = (Component)_dataSource.objectReferenceValue;
        if (component == null) return;

        // Cache the property list of the component.
        CachePropertyList(component.GetType());

        if (_propertyList.Length > 0)
        {
            // Show the property selection drop-down list.
            var index = Array.IndexOf(_propertyList, _propertyName.stringValue);
            var newIndex = EditorGUILayout.Popup("Property", Mathf.Max(0, index), _propertyList);

            // Update the property if the selection was changed.
            if (index != newIndex)
                _propertyName.stringValue = _propertyList[newIndex];
        }
        else
        {
            // No matched property found: Clear the previous selection
            _propertyName.stringValue = "";
        }
    }
    
    // Component list cache and its parent game object
    string[] _componentList;
    GameObject _cachedGameObject;

    // Property list cache and its parent type
    string[] _propertyList;
    Type _cachedType;

    // Hashed set of targetable types
    HashSet<Type> _targetableTypes = new HashSet<Type>(new [] {
        typeof(byte[]),
        typeof(bool),
        typeof(int), 
        typeof(float), 
        typeof(string),
        typeof(Vector2), 
        typeof(Vector3), 
        typeof(Vector4),
        typeof(Vector2Int), 
        typeof(Vector3Int)
    });

    // Cache components attached to the given game object if it's different
    // from the previously given one.
    void CacheComponentList(GameObject gameObject)
    {
        if (_cachedGameObject == gameObject) return;

        _componentList = gameObject.GetComponents<Component>().
            Select(x => x.GetType().Name).ToArray();

        _cachedGameObject = gameObject;
    }

    // Check the type of the given property.
    bool IsTargetable(PropertyInfo info)
    {
        return info.GetGetMethod() != null &&
               _targetableTypes.Contains(info.PropertyType);
    }

    // Cache properties from the given class if it's different from the
    // previously given one.
    void CachePropertyList(Type type)
    {
        if (_cachedType == type) return;

        _propertyList = type.GetProperties().
            Where(x => IsTargetable(x)).Select(x => x.Name).ToArray();

        _cachedType = type;
    }
}
