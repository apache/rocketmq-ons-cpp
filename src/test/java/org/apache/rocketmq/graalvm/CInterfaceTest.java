package org.apache.rocketmq.graalvm;

import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.junit.Assert;
import org.mockito.Mockito;

/**
 * <p>
 *     <strong>Warning:</strong> CTypeConversion can only used with native-image. Hence, we cannot create JUnit test
 *     cases in the Java side.
 * </p>
 */
public class CInterfaceTest {

//    @Test
    public void testCreateProducer() {
        CInterface.CFactoryProperty property = Mockito.mock(CInterface.CFactoryProperty.class);
        Mockito.when(property.getAccessKey()).thenReturn(CTypeConversion.toCString("Ak").get());
        Mockito.when(property.getAccessSecret()).thenReturn(CTypeConversion.toCString("Sk").get());
        Mockito.when(property.getGroupId()).thenReturn(CTypeConversion.toCString("GID_opensource_unit_test").get());
        Mockito.when(property.getNameServerAddress()).thenReturn(CTypeConversion.toCString("47.100.33.127:9876").get());
        int id = CInterface.create_producer(null, property);
        Assert.assertEquals(1, id);
        Assert.assertEquals(1, CInterface.instances.size());
    }

//    @Test
    public void testDestroyInstance() {
        CInterface.CFactoryProperty property = Mockito.mock(CInterface.CFactoryProperty.class);
        Mockito.when(property.getAccessKey()).thenReturn(CTypeConversion.toCString("Ak").get());
        Mockito.when(property.getAccessSecret()).thenReturn(CTypeConversion.toCString("Sk").get());
        Mockito.when(property.getGroupId()).thenReturn(CTypeConversion.toCString("GID_opensource_unit_test").get());
        Mockito.when(property.getNameServerAddress()).thenReturn(CTypeConversion.toCString("47.100.33.127:9876").get());
        int id = CInterface.create_producer(null, property);
        CInterface.destroy_instance(null, id);
        Assert.assertTrue(CInterface.instances.isEmpty());
    }
}